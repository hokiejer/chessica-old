#include "chess.h"



void Game::SearchThreadRoot(int ThreadID)
{
  ThreadData[ThreadID].Score = ((this)->*(Game::SearchRoutine))(ThreadID);
}


void * Game::SearchThreadLauncher(void * parms)
{
  SearchThreadParms *myparms = (SearchThreadParms *) parms;
  
  (myparms->myGame)->SearchThreadRoot(myparms->inThreadID);
}




void Game::LaunchAsyncMoveSearch()
{
  int ptrc, i;

  CurrentBoard->PrepareToSearch();
  for (i=0; i<NumThreads; i++)
  {
    //Thread parameters
    ThreadData[i].MoveCount = 0;

    //Parameters for launching thread
    ThreadData[i].ThreadParms.myGame = this;
    ThreadData[i].ThreadParms.inThreadID = i;
    //Note that I am not using the thread return value
    if (ptrc = pthread_create (&(ThreadData[i].SearchThread), NULL, &Game::SearchThreadLauncher, (void *) &(ThreadData[i].ThreadParms)))
      exit(ptrc);
  }
}

#ifdef COMMENT
void Game::LaunchAsyncMoveSearch2()
{
  int ptrc, i;

  SyncCounter = 0;  //Must be zeroed before search
  PrimeSearchThreadTrees();
  for (i=0; i<NumThreads; i++)
  {
    ThreadMoveCount[Ply][i] = 0;
    ThreadScore[i] = 0;
    ABParmlist[i].myTree = ThreadRoot[i];
printf("ThreadRoot[%d] == %x\n",i,ThreadRoot[i]);
    ABParmlist[i].inDepth = 20;
    ABParmlist[i].inLeaveDepth = 5;
    ABParmlist[i].inShowThinking = 1;
    ABParmlist[i].inThreadID = i + FIRST_SEARCH_THREAD;
    ABParmlist[i].inThreadLock = &ThreadLock;
    ABParmlist[i].inThreadStatus = &ThreadStatus;
    ABParmlist[i].inMoveCount = &(ThreadMoveCount[Ply][i]);
    ABParmlist[i].Retvalue = &(ThreadScore[i]);
    if (ptrc = pthread_create (&SearchThreads[i], NULL, &ResetTree::ParallelIterativeABLNSBStatic, (void *) &ABParmlist[i]))
      exit(ptrc);
  }
}
#endif



void Game::HaltAsyncMoveSearch()
{
  int i;

  for (i=0; i<NumThreads; i++)
  {
    if (CurrentBoard->AbortTreeSearch())
      pthread_join( ThreadData[i].SearchThread, NULL);
  }
}



void Game::WaitForMoveSearchToEnd()
{
  int i;

  for (i=0; i<NumThreads; i++)
  {
    pthread_join( ThreadData[i].SearchThread, NULL);
  }
}






