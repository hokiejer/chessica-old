#include "chess.h"






int ResetTree::AlphaBetaSaveBest(int Depth, int Ply, int Min, int Max, int ThreadID, long long int *NodeCount)
{
  ResetTree *CurrentNode, *NodePtr;
  ResetTree *OneMove = 0; //Not used to point to anything
  int TempScore, BestSoFar;
  ResetTree *AvoidMe = 0;

  if (CeaseProcessing)
    return (0);

  MoveHistory[Ply] = this;
  if (Depth == 0)
  {
    //PrintMoveHistory(Ply); PrintReset();
    (*NodeCount)++;
    return (ScoreResetNode());
  }
  else
  {
    //
    // Look at existing move first (Only one)
    //
    if (CurrentNode = Children.First)
    {
      AvoidMe = CurrentNode;
      BestMoveSaved = TRUE;

      TempScore = CurrentNode->AlphaBetaSaveBest(Depth-1,Ply+1,Min,Max,ThreadID,NodeCount);
      if (CeaseProcessing)
        return (0);

      if (ToMove == WHITE)
      {
        if (TempScore > Max) 
        {
          Max = TempScore;
        }
        if (Min <= Max)
        {
          //DeleteTree(ThreadID); //Delete all children
          return(Max);
        }
      }
      else //ToMove == BLACK
      {
        if (TempScore < Min) 
        {
          Min = TempScore;
        }
        if (Min <= Max)
        {
          DeleteTree(ThreadID); //If move got pruned, delete subtree
          return(Min);
        }
      }
    }

    InitializeMoveGeneration();

    //
    // Begin generating new moves
    //
    CurrentNode = NewOrphan(ThreadID);
    while(GenerateNextMove(CurrentNode))
    {
      OneMove = CurrentNode;	//nonzero - Should be in a register

      if (AvoidMe && CurrentNode->ChildrenMatch(AvoidMe))
      {
        //I've already scored this move, so chuck it
        AvoidMe = (ResetTree *) 0;
        InitMyChild(CurrentNode); //JMR just added this
      }
      else
      {
        TempScore = CurrentNode->AlphaBetaSaveBest(Depth-1,Ply+1,Min,Max,ThreadID,NodeCount);
        if (CeaseProcessing)
        {
          CurrentNode->DeleteMe(ThreadID);
          return (0);
        }

        if (ToMove == WHITE)
        {
          if (TempScore > Max) 
          {
            Max = TempScore;
            PutNodeFirstInList(CurrentNode); //Note that this adds the node and its children
            PruneAllButFirst(ThreadID);
            BestMoveSaved = TRUE;
            AvoidMe = (ResetTree *) 0;
            CurrentNode = NewOrphan(ThreadID);
          }
          else
          {
            CurrentNode->DeleteTree(ThreadID);
            InitMyChild(CurrentNode);
          }
        }
        else //ToMove == BLACK
        {
          if (TempScore < Min) 
          {
            Min = TempScore;
            PutNodeFirstInList(CurrentNode); //Note that this adds the node and its children
            PruneAllButFirst(ThreadID);
            BestMoveSaved = TRUE;
            AvoidMe = (ResetTree *) 0;
            CurrentNode = NewOrphan(ThreadID);
          }
          else
          {
            CurrentNode->DeleteTree(ThreadID);
            InitMyChild(CurrentNode);
          }
        }
        if (Min <= Max)
        {
          DeleteTree(ThreadID); //If move got pruned, delete subtree
          break;
        }
      }
    }
    CurrentNode->DeleteMe(ThreadID); //Unused orphan

    if (OneMove == 0) //If there were no children
    {
      //Do I need to set ScoreDepth here?
      return(ScoreResetNode());
    }
    else
    {
      if (ToMove == WHITE)
        return(Max);
      else
        return(Min);
    }
  }
}




int ResetTree::IterativeAlphaBetaSaveBest(int Depth, long long int *MoveCount)
{
  int i;
  int retval;

  for (i=1;i<=Depth;i++)
  {
    retval = AlphaBetaSaveBest(i,0,MAX_SCORE,MIN_SCORE,0,MoveCount); //$$$ hardcoded thread to 0
  }

  return(retval);
}




int Game::IterativeAlphaBetaSaveBest(int ThreadID)
{
  return (CurrentBoard->IterativeAlphaBetaSaveBest(ABParmlist.Depth,&(MoveCount[MoveNumber])));
}




////////////////////////////////////////////////
//
// The following is the AB currently in use
//
////////////////////////////////////////////////
// NoMovesGenerated() will be useful here


int ResetTree::AlphaBetaLeaveNodesSaveBest(int LeaveDepth, int Depth, int Ply, int Min, int Max, int ThreadID, long long int *NodeCount)
{
  ResetTree *CurrentNode, *NodePtr;
  ResetTree *OneMove = 0; //Not used to point to anything
  ResetTree *AvoidMe = (ResetTree *) 0;
  int TempScore, BestSoFar;

  if (CeaseProcessing) //This needs to return something other than zero
    return (0);

  MoveHistory[Ply] = this;
  if (Depth == 0)
  {
    //PrintMoveHistory(Ply); PrintReset();
    (*NodeCount)++;
    return (ScoreResetNode());
  }
  else
  {
    //
    // Look at existing moves first
    //
    if (CurrentNode = Children.First)
    {
      OneMove = CurrentNode;
      while(CurrentNode)
      {
        if (Ply < LeaveDepth)
          TempScore = CurrentNode->AlphaBetaLeaveNodesSaveBest(LeaveDepth,Depth-1,Ply+1,Min,Max,ThreadID,NodeCount);
        else
          TempScore = CurrentNode->AlphaBetaSaveBest(Depth-1,Ply+1,Min,Max,ThreadID,NodeCount);
        if (CeaseProcessing)
          return (0);

        if (ToMove == WHITE)
        {
          if (TempScore > Max) 
          {
            Max = TempScore;
            if (CurrentNode != Children.First)
            {
              NodePtr = CurrentNode->NextSibling;
              DequeueChildNotFirst(CurrentNode);
              PutNodeFirstInList(CurrentNode);
              CurrentNode = NodePtr;
            }
            else
            {
              CurrentNode = CurrentNode->NextSibling;
            }
          }
          else
          {
            CurrentNode = CurrentNode->NextSibling; 
          }
        }
        else //ToMove == BLACK
        {
          if (TempScore < Min) 
          {
            Min = TempScore;
            if (CurrentNode != Children.First)
            {
              NodePtr = CurrentNode->NextSibling;
              DequeueChildNotFirst(CurrentNode);
              PutNodeFirstInList(CurrentNode);
              CurrentNode = NodePtr;
            }
            else
            {
              CurrentNode = CurrentNode->NextSibling;
            }
          }
          else
          {
            CurrentNode = CurrentNode->NextSibling;
          }
        }
        if (Min <= Max)
          break;
      }
    }
    else //no children
    {
      InitializeMoveGeneration();
    }

    if (Min > Max)
    {
      if (BestMoveSaved)
      {
        BestMoveSaved = FALSE;
        InitializeMoveGeneration();
        AvoidMe = Children.First;
      }

      //
      // Begin generating new moves
      //
      CurrentNode = NewOrphan(ThreadID);
      while(GenerateNextMove(CurrentNode))
      {
        OneMove = CurrentNode;	//nonzero - Should be in a register
 
        if (AvoidMe && CurrentNode->ChildrenMatch(AvoidMe))
        {
          //Skip this move
          AvoidMe = (ResetTree *) 0;
          InitMyChild(CurrentNode);
        }
        else
        {
          if (Ply < LeaveDepth)
            TempScore = CurrentNode->AlphaBetaLeaveNodesSaveBest(LeaveDepth,Depth-1,Ply+1,Min,Max,ThreadID,NodeCount);
          else
            TempScore = CurrentNode->AlphaBetaSaveBest(Depth-1,Ply+1,Min,Max,ThreadID,NodeCount);
          if (CeaseProcessing)
          {
            CurrentNode->DeleteMe(ThreadID);
            return (0);
          }
  
          if (ToMove == WHITE)
          {
            if (TempScore > Max) 
            {
              Max = TempScore;
              PutNodeFirstInList(CurrentNode);
            }
            else
            {
              PutNodeLastInList(CurrentNode);
            }
          }
          else //ToMove == BLACK
          {
            if (TempScore < Min) 
            {
              Min = TempScore;
              PutNodeFirstInList(CurrentNode);
            }
            else
            {
              PutNodeLastInList(CurrentNode);
            }
          }
          if (Min <= Max)
            break;
          CurrentNode = NewOrphan(ThreadID);
        } 		//not avoiding anything
      } 		//end while
      if ((Children.First != CurrentNode) && (Children.Last != CurrentNode))
        CurrentNode->DeleteMe(ThreadID); //Unused orphan
    }
    if (OneMove == 0) //If there were no children
    {
      //Do I need to set ScoreDepth here?
      return(ScoreResetNode());
    }
    else
    {
      if (ToMove == WHITE)
        return(Max);
      else
        return(Min);
    }
  }
}


int ResetTree::IterativeAlphaBetaLeaveNodesSaveBest(int Depth, int LeaveDepth, int ShowThinking, int ThreadID, long long int *MoveCount)
{
  int i;
  int retval;
  ResetTree *printptr;
  char mybuffer[20];

  CeaseProcessing = 0;

  printf("This == %x\n",this);
  for (i=1;i<=Depth;i++)
  {
    if (!CeaseProcessing)
    {
      retval = AlphaBetaLeaveNodesSaveBest(LeaveDepth,i,      0,MAX_SCORE,MIN_SCORE,ThreadID,MoveCount);
      //ply score time nodes text
      if (ShowThinking)
      {
        IOLock.Obtain();
        printf("%d %d 0 %d ",i,retval / 10,*MoveCount);
        printptr = Children.First;
        while (printptr)
        {
          printptr->GetAlgebraicNotation(printptr->Parent,mybuffer);
          printf("%s  ",mybuffer);
          printptr = printptr->Children.First;
        }
        printf("\n");
        IOLock.Release();
      }
    }
  }

  return(retval);
}



int ResetTree::ParallelIterativeAlphaBetaLeaveNodesSaveBest(int Depth, int LeaveDepth, int ShowThinking, int ThreadID, Lock *ThreadLock, int *ThreadStatus, long long int *MoveCount)
{
  int i;
  int retval;
  ResetTree *printptr;
  char mybuffer[20];

  CeaseProcessing = 0;

  for (i=1;i<=Depth;i++)
  {
    if (!CeaseProcessing)
    {
      retval = AlphaBetaLeaveNodesSaveBest(LeaveDepth,i,      0,MAX_SCORE,MIN_SCORE,ThreadID,MoveCount);
      
      ThreadLock->Obtain();
   
      //If I am last to the party
      if (((*ThreadStatus + 1) % SEARCH_THREADS) == 0)
      {
        if (ShowThinking)
        {
          //Find and print best score
          IOLock.Obtain();
          printf("%d %d 0 %d ",i,retval / 10,*MoveCount);
          printptr = Children.First;
          while (printptr)
          {
            printptr->GetAlgebraicNotation(printptr->Parent,mybuffer);
            printf("%s  ",mybuffer);
            printptr = printptr->Children.First;
          }
          printf("\n");
          IOLock.Release();
        }
      }
      
      (*ThreadStatus)++;

      ThreadLock->Release();
      
      while (*ThreadStatus < (SEARCH_THREADS*i))
        usleep(100);
    }
  }

  return(retval);
}

#ifdef COMMENT
void * ResetTree::IterativeABLNSBStatic(void * parms)
{
  ABParms *myparms = (ABParms *) parms;

  *(myparms->Retvalue) = (myparms->myTree)->IterativeAlphaBetaLeaveNodesSaveBest(myparms->inDepth,myparms->inLeaveDepth,myparms->inShowThinking,myparms->inThreadID,myparms->inMoveCount);
}



void * ResetTree::ParallelIterativeABLNSBStatic(void * parms)
{
  ABParms *myparms = (ABParms *) parms;

  *(myparms->Retvalue) = (myparms->myTree)->ParallelIterativeAlphaBetaLeaveNodesSaveBest(myparms->inDepth,myparms->inLeaveDepth,myparms->inShowThinking,myparms->inThreadID,myparms->inThreadLock,myparms->inThreadStatus,myparms->inMoveCount);
}
#endif



int Game::ParallelIterativeAlphaBetaLeaveNodesSaveBest(int ThreadID)
{
  int i;
  int retval;
  ResetTree *printptr;
  char mybuffer[20];

  ThreadLock.Obtain();
  //If I am first to the party
  if ((SyncCounter % NumThreads) == 0)
  { 
    PrimeSearchThreadTrees();
  }
  SyncCounter++;
  ThreadLock.Release();
  
  while (SyncCounter < (NumThreads*i))
    usleep(100);
    
  for (i=1;i<=ABParmlist.Depth;i++)
  {
    if (ThreadData[ThreadID].Root->StillSearching())
    {
      ThreadData[ThreadID].Score = ThreadData[ThreadID].Root->AlphaBetaLeaveNodesSaveBest(ABParmlist.LeaveDepth,i,      0,MAX_SCORE,MIN_SCORE,ThreadID,&(ThreadData[ThreadID].MoveCount));
      
      ThreadLock.Obtain();
   
      //If I am last to the party
      if (((SyncCounter + 1) % NumThreads) == 0)
      {
        if (ShowThinking)
        {
          //Find best score
          
          
          IOLock.Obtain();
          
          //print best score - this needs to be a resettree routine
          printf("%d %d 0 %d ",i,ThreadData[ThreadID].Score / 10,ThreadData[ThreadID].MoveCount);
          ThreadData[ThreadID].Root->PrintBestMoveLine();
          IOLock.Release();
        }
      }
      
      SyncCounter++;

      ThreadLock.Release();
      
      while (SyncCounter < (NumThreads*i))
        usleep(100);
    }
  }
  MergeSearchThreadTrees();
  return(retval);
}


