#include "chess.h"
#include <pthread.h>

Game::Game()	//constructor
{
  ProgramActive = 1;
  EngineRequest = 0;
  WhitePlayer = HUMAN_PLAYER;
  BlackPlayer = COMPUTER_PLAYER;
  Ply = 0;
  MoveNumber = 0;
  XBoard = 0;
  Ponder = 1;

  //Set Search algorithm
  SearchRoutine = &Game::ParallelIterativeAlphaBetaLeaveNodesSaveBest;
  NumThreads = 3;

  //Search parameters
  ABParmlist.Depth = 20;
  ABParmlist.LeaveDepth = 5;
}


void Game::InitGame()
{
  SetGame("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}


void Game::SetGame(const char * FEN)
{
  long long int MC;
  long long int *MoveCount;
  int retval;

  if (CurrentBoard->AbortTreeSearch())
  {
    //printf ("InitGame found a tree search running???\n");
  }
  if (InitialBoard)
  {
    InitialBoard->DeleteTree(IO_THREAD);
    InitialBoard->DeleteMe(IO_THREAD);
  }
  InitialBoard = InitialBoard->GetNode(IO_THREAD); //This syntax stinks - I need to fix it
  CurrentBoard = InitialBoard;
  CurrentBoard->DeleteTree(IO_THREAD);
  CurrentBoard->bInitToFEN(FEN);
  //CurrentBoard->ScoreResetNode(); //I don't see an actual reason for this call
  CurrentBoard->InitializeMoveGeneration();

  //Ensure that first level of moves are generated
  CurrentBoard->GenerateMoves(1,IO_THREAD);
printf("AB has been called\n");
}


//Initialization performed only when XBoard calls the chess engine
void Game::InitXBoardGame()
{
  XBoard = 1;
}



void Game::StartGame()
{
  long int a, b, c, d;

  CurrentBoard->bInitToFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  CurrentBoard->ScoreResetNode();

  CurrentBoard->PrintReset();
  while(CurrentBoard->GameNotOver())
  {
    LaunchAsyncMoveSearch();

    sleep(5);

    HaltAsyncMoveSearch();

    CurrentBoard = CurrentBoard->UseBestMove();
    CurrentBoard->PrintReset();
  }
}

int Game::ComputerToMove()
{
  return (((CurrentBoard->WhiteToMove()) && (WhitePlayer == COMPUTER_PLAYER)) ||
      ((!(CurrentBoard->WhiteToMove())) && (BlackPlayer == COMPUTER_PLAYER)));
}


void Game::GameCleanup()
{
  InitialBoard->DeleteTree(IO_THREAD);
  InitialBoard->DeleteMe(IO_THREAD);
}


void Game::PrimeSearchThreadTrees()
{
  int i;
  ResetTree *TempNode;

  int retval;

  //Set up thread parent nodes
  for (i=0;i<NumThreads;i++)
  {
    ThreadData[i].Root = CurrentBoard->GetNode(ENGINE_THREAD);
    CurrentBoard->CopyReset(ThreadData[i].Root);
    ThreadData[i].Nodes = 0;
  }
  TotalNodes = 0;

  //Distribute children to thread parent nodes
  i = 0;
  while (TempNode = CurrentBoard->DequeueFirstChild())
  {
    i = i % NumThreads;
    ThreadData[i].Root->PutNodeLastInList(TempNode);
    (ThreadData[i].Nodes)++;
    TotalNodes++;
    i++;
  }
  printf("CurrentBoard has no more children. TotalNodes == %d.\n",TotalNodes);
  for (i=0;i<NumThreads;i++)
  {
    printf("Thread: %d, ThreadNodes: %d\n",i,ThreadData[i].Nodes);
    ThreadData[i].Root->PrintMoveOptions();
  }

  //AT THIS POINT, CURRENTBOARD HAS NO CHILDREN!
}


void Game::MergeSearchThreadTrees()
{
  int i,j;
  int temp;
  int SortedIndices[SEARCH_THREADS];
  int SortedScores[SEARCH_THREADS];
  int NumThreads;
  ResetTree *TempNode;

  //Initialize for sorting
  j=0;
  for (i=0;i<NumThreads;i++)
  {
    if (ThreadData[i].Nodes)
    {
      SortedIndices[j] = i;
      SortedScores[j++] = ThreadData[i].Score;
    }
  }
  NumThreads = j;
  
  //Sort scores best to last
  for (i=0;i<NumThreads-1;i++)
  {
    for (j=i+1;j<NumThreads;j++)
    {
      if (((CurrentBoard->WhiteToMove()) && (SortedScores[j] > SortedScores[i])) ||
          (!(CurrentBoard->WhiteToMove()) && (SortedScores[j] < SortedScores[i])))
      {
        temp = SortedScores[i];
        SortedScores[i] = SortedScores[j];
        SortedScores[j] = temp;
        temp = SortedIndices[i];
        SortedIndices[i] = SortedIndices[j];
        SortedIndices[j] = temp;
      }
    }
  }
  
  //Merge the threads back to CurrentBoard
  while (TotalNodes)
  {
    for (i=0;i<NumThreads;i++)
    {
      if (TempNode = (ThreadData[SortedIndices[i]].Root)->DequeueFirstChild())
      {
        TotalNodes--;
        (ThreadData[SortedIndices[i]].Nodes)--;
        CurrentBoard->PutNodeLastInList(TempNode);
      }
    }
  }
}


