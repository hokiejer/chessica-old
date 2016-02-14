#include "chess.h"


int ResetTree::SerialAlphaBeta(int Depth, int Ply, int Min, int Max, long long int *NodeCount)
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
    MyChild = NewOrphan(RUNS_SERIALLY);
    while(GenerateNextMove(MyChild))
    {
      OneMove = Ply+1;	//nonzero - Should be in a register

      TempScore = MyChild->SerialAlphaBeta(Depth-1,Ply+1,Min,Max,NodeCount);
      if (ToMove == WHITE)
      {
        if (TempScore > Max) 
        {
          Max = TempScore;
        }
      }
      else //ToMove == BLACK
      {
        if (TempScore < Min) 
        {
          Min = TempScore;
        }
      }
      if (Min <= Max)
        break;
      InitMyChild(MyChild);
    }
    MyChild->DeleteMe(RUNS_SERIALLY); 

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


int Game::SerialAlphaBeta(int ThreadID)
{
  //Serial
  if (ThreadID == 0)
  {
    printf("serial ab\n");
    SearchScore[Ply] = CurrentBoard->SerialAlphaBeta(ABParmlist.Depth,0,MAX_SCORE,MIN_SCORE,&(MoveCount[Ply]));
  }
}


int ResetTree::SerialAlphaBetaOnList(int Depth, int Ply, int Min, int Max, long long int *NodeCount)
{
  ResetTree *CurrentNode, *NodePtr;
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
    CurrentNode = Children.First;
    while(CurrentNode)
    {
      if (CurrentNode->Children.First)
      {
        TempScore = CurrentNode->SerialAlphaBetaOnList(Depth-1,Ply+1,Min,Max,NodeCount);
      }
      else
      {
        TempScore = CurrentNode->SerialAlphaBeta(Depth-1,Ply+1,Min,Max,NodeCount);
      }
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
            CurrentNode = CurrentNode->NextSibling; //JMR - just added
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
            CurrentNode = CurrentNode->NextSibling; //JMR - just added
        }
        else
        {
          CurrentNode = CurrentNode->NextSibling;
        }
      }
      if (Min <= Max)
        break;
    }
    if (ToMove == WHITE)
      return(Max);
    else
      return(Min);
  }
}


int ResetTree::IterativeAlphaBeta(int PrimeDepth, int Depth, long long int *MoveCount)
{
  int i;
  int retval;

  // Generate the moves list
  GenerateMoves(PrimeDepth,RUNS_SERIALLY);

  for (i=1;i<=Depth;i++)
  {
    retval = SerialAlphaBetaOnList(i,0,MAX_SCORE,MIN_SCORE,MoveCount);
  }

  DeleteTree(RUNS_SERIALLY);

  return(retval);
}


int Game::IterativeAlphaBeta(int ThreadID)
{
  //Serial
  if (ThreadID == 0)
  {
    SearchScore[Ply] = CurrentBoard->IterativeAlphaBeta(ABParmlist.PrimeDepth,ABParmlist.Depth,&(MoveCount[Ply]));
  }
}




int ResetTree::AlphaBetaCapturesOnly(int Min, int Max, long long int *NodeCount)
{
  ResetTree *MyChild, *CurrentNode;
  int OneMove = 0;
  int TempScore, BestSoFar;

  (*NodeCount)++;
  InitializeMoveGeneration();
  MyChild = NewOrphan(RUNS_SERIALLY);
  while(GenerateNextCapture(MyChild))
  {
    OneMove = 1;	//nonzero - Should be in a register

    TempScore = MyChild->AlphaBetaCapturesOnly(Min,Max,NodeCount);
    if (ToMove == WHITE)
    {
      if (TempScore > Max) 
      {
        Max = TempScore;
      }
    }
    else //ToMove == BLACK
    {
      if (TempScore < Min) 
      {
        Min = TempScore;
      }
    }
    if (Min <= Max)
      break;
    InitMyChild(MyChild);
  }
  MyChild->DeleteMe(RUNS_SERIALLY); 

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





int ResetTree::AlphaBetaInPlace(int MaxHold, int Depth, int Ply, int Min, int Max, long long int *NodeCount)
{
  ResetTree *CurrentNode, *NodePtr;
  ResetTree *OneMove = 0; //Not used to point to anything
  int TempScore, BestSoFar;

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
    // Look at existing moves first
    //
    if (CurrentNode = Children.First)
    {
      OneMove = CurrentNode;
      while(CurrentNode)
      {
        if (Ply < MaxHold)
          TempScore = CurrentNode->AlphaBetaInPlace(MaxHold,Depth-1,Ply+1,Min,Max,NodeCount);
        else
          TempScore = CurrentNode->SerialAlphaBeta(Depth-1,Ply+1,Min,Max,NodeCount);
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
              CurrentNode = CurrentNode->NextSibling; //JMR - just added
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
              CurrentNode = CurrentNode->NextSibling; //JMR - just added
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
      //
      // Begin generating new moves
      //
      CurrentNode = NewOrphan(RUNS_SERIALLY);
      while(GenerateNextMove(CurrentNode))
      {
        OneMove = CurrentNode;	//nonzero - Should be in a register
  
        if (Ply < MaxHold)
          TempScore = CurrentNode->AlphaBetaInPlace(MaxHold,Depth-1,Ply+1,Min,Max,NodeCount);
        else
          TempScore = CurrentNode->SerialAlphaBeta(Depth-1,Ply+1,Min,Max,NodeCount);
        if (CeaseProcessing)
        {
          CurrentNode->DeleteMe(RUNS_SERIALLY);
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
        CurrentNode = NewOrphan(RUNS_SERIALLY);
      }
      if ((Children.First != CurrentNode) && (Children.Last != CurrentNode))
        CurrentNode->DeleteMe(RUNS_SERIALLY); //Unused orphan
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


int ResetTree::IterativeAlphaBetaInPlace(int Depth, int MaxHold, long long int *MoveCount)
{
  int i;
  int retval;

  CeaseProcessing = 0;

  for (i=1;i<=Depth;i++)
  {
    retval = AlphaBetaInPlace(MaxHold,i,0,MAX_SCORE,MIN_SCORE,MoveCount);
    //printf("Depth == %d, Total Nodes == %d\n",i,CountTree());
    //printf("Search Depth == %d\n",i);
  }

  return(retval);
}



int Game::IterativeAlphaBetaInPlace(int ThreadID)
{
  //Serial
  if (ThreadID == 0)
  {
    SearchScore[Ply] = CurrentBoard->IterativeAlphaBetaInPlace(ABParmlist.Depth,ABParmlist.MaxHold,&(MoveCount[Ply]));
  }
}



#ifdef COMMENT
void * ResetTree::IterativeABIPStatic(void * parms)
{
  ABParms *myparms = (ABParms *) parms;

  *(myparms->Retvalue) = (myparms->myTree)->IterativeAlphaBetaInPlace(myparms->inDepth,myparms->inMaxHold,myparms->inMoveCount);
}
#endif

