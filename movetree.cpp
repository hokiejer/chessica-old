#include "chess.h"



void ResetTree::GenerateMoves(int Depth, int ThreadID)
{
  ResetTree *MyChild;

  if (Depth == 0)
    return;

  InitializeMoveGeneration();

  MyChild = NewOrphan(ThreadID);
  while(GenerateNextMove(MyChild))
  {
    MyChild->GenerateMoves(Depth-1,ThreadID);
    PutNodeLastInList(MyChild);

    MyChild = NewOrphan(ThreadID);
  }

  MyChild->DeleteMe(ThreadID); //if there is one

}




int ResetTree::SerialMoveTree(int Depth, int Ply, long long int *NodeCount)
{
  ResetTree *MyChild, *CurrentNode;
  int OneMove = 0;
  int TempScore, BestSoFar;

  MoveHistory[Ply] = this;
  if (Depth == 0)
  {
    //PrintMoveHistory(Ply); PrintReset();
    (*NodeCount)++;
    return (ScoreResetNode());
  }
  else
  {
    InitializeMoveGeneration();
    if (ToMove == WHITE)
      BestSoFar = (int) -MAX_SCORE;
    else //ToMove == BLACK
      BestSoFar = (int) MAX_SCORE;

    MyChild = NewOrphan(RUNS_SERIALLY); //$$$ hardcoded to zero
    while(GenerateNextMove(MyChild))
    {
      OneMove = Ply+1;	//nonzero - Should be in a register

      TempScore = MyChild->SerialMoveTree(Depth-1,Ply+1,NodeCount);
      if (ToMove == WHITE)
      {
        if (TempScore > BestSoFar) 
        {
          BestSoFar = TempScore;
        }
      }
      else //ToMove == BLACK
      {
        if (TempScore < BestSoFar) 
        {
          BestSoFar = TempScore;
        }
      }
      InitMyChild(MyChild);
    }
    MyChild->DeleteMe(RUNS_SERIALLY); 

    if (OneMove == 0) //If there were no children
    {
      //Do I need to set ScoreDepth here?
      return(ScoreResetNode());
    }
    else
      return(BestSoFar);
  }
}

int ResetTree::MoveTree(int Depth, int Ply, long long int *NodeCount, int ThreadID)
{
  ResetTree *MyChild, *CurrentNode;
  int OneMove = 0;

//printf("MoveTree(%d,%d,ptr)\n",Depth,Ply);
  //MoveHistory[Ply] = this;
  if (Depth == 0)
  {
    ScoreResetNode();
    //PrintMoveHistory(Ply); PrintReset();
    (*NodeCount)++;
  }
  else
  {
    if (Ply == 0)
    {
      //Ensure that the children are created
      Serialize();
      CurrentNode = NewOrphan(RUNS_SERIALLY); //$$$ hardcoded to zero
      while(GenerateNextMove(CurrentNode))
      {
        PutNodeLastInList(CurrentNode);
        CurrentNode->InitializeSerialization();
        CurrentNode = NewOrphan(RUNS_SERIALLY); //$$$ hardcoded to zero
      }
      CurrentNode->DeleteMe(ThreadID);
      Unserialize(); 

      //Try to serialize a child and work it
      CurrentNode = Children.First;
      while(CurrentNode)
      {
        if (CurrentNode->Serialize_Or_Skip())
        {
          OneMove = Ply+1;	//nonzero - Should be in a register
          CurrentNode->SerialMoveTree(Depth-1,Ply+1,NodeCount);
          //Leave this serialized
        }
        CurrentNode = CurrentNode->NextSibling;
      }
    }
    else //No serialization necessary
    {
      InitializeMoveGeneration();
      MyChild = NewOrphan(RUNS_SERIALLY); //$$$ hardcoded to zero
      while(GenerateNextMove(MyChild))
      {
        OneMove = Depth;	//nonzero - Depth should be in a register
        MyChild->SerialMoveTree(Depth-1,Ply+1,NodeCount);
        InitMyChild(MyChild);
      }
      MyChild->DeleteMe(ThreadID); 
    }
  }
}

void * ResetTree::MoveTreeStatic(void * parms)
{
  MoveTreeParms *myparms = (MoveTreeParms *) parms;

  *(myparms->Retvalue) = (myparms->myTree)->MoveTree(myparms->inDepth,myparms->inPly,myparms->inNodeCount,myparms->ThreadID);

}


int ResetTree::ParallelMoveTree(int Depth, int Ply, long long int *NodeCount)
{
  pthread_t thread[MAX_THREADS];
  int rc[MAX_THREADS];
  long long int ThreadNodeCount[MAX_THREADS];
  MoveTreeParms parms[MAX_THREADS];
  int Retvalue[MAX_THREADS];
  int i;

  InitializeSerialization();
  for (i=0;i<MAX_THREADS;i++)
  {
    parms[i].myTree = this; 
    parms[i].inDepth = Depth;
    parms[i].inPly = Ply;
    ThreadNodeCount[i] = 0;
    parms[i].inNodeCount = &(ThreadNodeCount[i]);
    parms[i].ThreadID = i;
    parms[i].Retvalue = &(Retvalue[i]);

    if (rc[i] = pthread_create (&(thread[i]), NULL, &ResetTree::MoveTreeStatic, (void *) &(parms[i])))
      printf("Thread creation %d failed: %d\n", i, rc[i]);
  } 

  //////////////////////////////
  // Wait for threads to finish
  //////////////////////////////
  
  for (i=0;i<MAX_THREADS;i++)
  {
    pthread_join( thread[i], NULL);
    *NodeCount += ThreadNodeCount[i];
  }
  DeleteChildren(RUNS_SERIALLY);
}



void ResetTree::PruneAllButFirst(int ThreadID)
{
  ResetTree *CurrentNode;

  while ((CurrentNode = Children.Last) != Children.First)
  {
    DequeueChildNotFirst(CurrentNode);
    CurrentNode->DeleteTree(ThreadID);
    CurrentNode->DeleteMe(ThreadID);
  }
}


ResetTree * ResetTree::UseBestMove()
{
  if (GameNotOver())
  {
    PruneAllButFirst(ENGINE_THREAD);
  }
  return (Children.First);
}


ResetTree * ResetTree::UseThisMove(char fromsquare, char tosquare, int promotion)
{
  ResetTree *CurrentNode;

  CurrentNode = FindMatchingChild(fromsquare,tosquare);

  if (CurrentNode)
  {
    DequeueChild(CurrentNode);
    PutNodeFirstInList(CurrentNode);
    return (UseBestMove());
  }
  else 
    return((ResetTree *) 0);
}



int ResetTree::MoveIsValid(char fromsquare, char tosquare, int promotion, int ThreadID)
{
  unsigned long long int Bitmask = 0x8000000000000000;
  ResetTree *LocalParent, *LocalChild;

  Bitmask = Bitmask >> To;

  LocalParent = GetNode(ThreadID);
  CopyReset(LocalParent);

  LocalChild = LocalParent->NewOrphan(ThreadID);

  LocalParent->InitializeMoveGeneration();
  while(LocalParent->GenerateNextMove(LocalChild))
  {
    if ((LocalChild->To == tosquare) && (LocalChild->From == fromsquare))
    {
      if (promotion)
      {
        if (((promotion == 1) && (Bitmask & LocalChild->bBishops)) ||
            ((promotion == 2) && (Bitmask & LocalChild->bKnights)) ||
            ((promotion == 3) && (Bitmask & LocalChild->bRooks)) ||
            ((promotion == 4) && (Bitmask & LocalChild->bQueens)))
        {
          LocalParent->DeleteMe(ThreadID);
          LocalChild->DeleteMe(ThreadID);
          return(1);
        }
      }
      else
      {
        LocalParent->DeleteMe(ThreadID);
        LocalChild->DeleteMe(ThreadID);
        return(1);
      }
    }
  }
  LocalParent->DeleteMe(ThreadID);
  LocalChild->DeleteMe(ThreadID);
  return(0);
}



void ResetTree::PrintMoveOptions()
{
  ResetTree *CurrentNode = Children.First;
  char mybuffer[20], mybuffer2[20];
  int i = 1;

  printf("There are %d children:\n\n",CountChildren());
  while(CurrentNode)
  {
    CurrentNode->GetMoveText(mybuffer);
    //CurrentNode->GetAlgebraicNotation(mybuffer2);
    //printf("%d. %s (%s)\n",i++,mybuffer,mybuffer2);
    printf("%d. %s  \t",i++,mybuffer);
    CurrentNode = CurrentNode->NextSibling;
    if ((i-1) % 5 == 0)
      printf("\n");
  }
  printf("\n");
}


ResetTree *ResetTree::ConsoleDebug(char *DebugString)
{
  if ((strcmp(DebugString,"help")) == 0)
  {
    IOLock.Obtain();
    printf("  Available Options:\n");
    printf("    PrintReset: Full Reset Printout\n");
    printf("    PrintBoard: Small Board Printout\n");
    printf("    PrintMoveOptions\n");
    printf("    ResetDebugPtr: Move DebugPtr back to the current board\n");
    printf("    FirstChild: Move DebugPtr to first Child\n");
    printf("    NextChild: Move DebugPtr to next child\n");
    printf("    PrevChild: Move DebugPtr to previous child\n");
    IOLock.Release();
  }
  if ((strcmp(DebugString,"PrintBoard")) == 0)
  {
    PrintSmallBoard();
    return(this);
  }
  if ((strcmp(DebugString,"PrintReset")) == 0)
  {
    PrintReset();
    return(this);
  }
  if ((strcmp(DebugString,"PrintMoveOptions")) == 0)
  {
    PrintMoveOptions();
    return(this);
  }
  if ((strcmp(DebugString,"FirstChild")) == 0)
  {
    return(Children.First);
  }
  if ((strcmp(DebugString,"NextChild")) == 0)
  {
    return(NextSibling);
  }
  if ((strcmp(DebugString,"PrevChild")) == 0)
  {
    return(PrevSibling);
  }
  if ((strcmp(DebugString,"TreeDiags")) == 0)
  {
    TreeDiagnostics();
    return(this);
  }
  printf("ConsoleDebug: No match for string \"%s\"!\n",DebugString);
}



