#include "chess.h"

//////////////////////////////////////////////////////////////////
//
// This file contains routines necessary to launch and run the
// Computer Thought thread that will govern pondering and moving
//
//////////////////////////////////////////////////////////////////

void Game::LaunchEngine()
{
  int ptrc;
  int retcode;

  EngineStatus = 0;
  EngineParmList.ThisGame = this;
  EngineParmList.Retvalue = &retcode;
  if (ptrc = pthread_create (&EngineThread, NULL, &Game::EngineStatic, (void *) &EngineParmList))
    exit(ptrc);
}

void Game::KillEngine()
{
  ProgramActive = 0;
  pthread_join (EngineThread, NULL);
}


void * Game::EngineStatic(void * parms)
{
  EngineParms *myparms = (EngineParms *) parms;

  *(myparms->Retvalue) = (myparms->ThisGame)->Engine();
}


void Game::MakeMove()
{
  char fromtext[2], totext[2];
  int fromsquare, tosquare, promotion;
  char mybuffer[10];
  long long int MC;
  long long int *MoveCount;
  float retval;
  
  ResetTree *tempptr = InitialBoard; //for debug
  char treediags[] = "TreeDiags"; //for debug

  if (ComputerToMove())
  {
    //Computer's move
    CurrentBoard = CurrentBoard->UseBestMove();
    tempptr = tempptr->ConsoleDebug(treediags); //debug
    IOLock.Obtain();
    CurrentBoard->GetMoveText(mybuffer);
    printf("move %s\n",mybuffer);
    IOLock.Release();
  }
  else 
  {
    //Human's move
    fromtext[0] = UserMove[0];
    fromtext[1] = UserMove[1];
    fromsquare = TextToSquareNumber(fromtext);
    totext[0] = UserMove[2];
    totext[1] = UserMove[3];
    tosquare = TextToSquareNumber(totext);
    if (UserMove[4] == 'n')
      promotion = 1;
    if (UserMove[4] == 'b')
      promotion = 2;
    if (UserMove[4] == 'r')
      promotion = 3;
    if (UserMove[4] == 'q')
      promotion = 4;
    CurrentBoard = CurrentBoard->UseThisMove(fromsquare,tosquare,promotion);
  }

  //Generic move stuff
  Ply++;
  if (CurrentBoard->WhiteToMove());
    MoveNumber++;
  DebugPtr = CurrentBoard;

  //Ensure that first level of moves are generated
  CurrentBoard->GenerateMoves(1,ENGINE_THREAD);
  //retval = CurrentBoard->AlphaBetaLeaveNodesSaveBest(1,1,0,MAX_SCORE,MIN_SCORE,0,MoveCount); //$$$ I'm nervous about this thread usage

  //$$$ Note that when the computer ponders, these children will be given to threads.
  //$$$ If a human move is received, this means I have to move the nodes back first
}


int Game::Engine()
{
    
  printf("Engine is alive!\n");
  while (ProgramActive)
  {
    switch(EngineRequest)
    {
      case ENGINEACTION_MOVE:
          //Stop thinking if I was thinking
          if (EngineStatus == ENGINEACTION_THINK)
          {
            HaltAsyncMoveSearch();
            EngineStatus = ENGINEACTION_WAIT;
          }
        //This is not a steady state.  It transitions directly to MOVE or WAIT
          MakeMove();
          if (Ponder || ComputerToMove())
            EngineRequest = ENGINEACTION_THINK;
          else
            EngineRequest = ENGINEACTION_WAIT;


      case ENGINEACTION_WAIT: //0
        if (EngineStatus != ENGINEACTION_WAIT)
        {
          if (EngineStatus == ENGINEACTION_THINK)
          {
            //Stop tree generation - this ends the thread
            HaltAsyncMoveSearch();
            EngineStatus = ENGINEACTION_WAIT;
          }
        }
        break;


      case ENGINEACTION_THINK:
        if (EngineStatus != ENGINEACTION_THINK)
        {
          if (EngineStatus == ENGINEACTION_WAIT)
          {
            //Start tree generation
            LaunchAsyncMoveSearch();
            EngineStatus = ENGINEACTION_THINK;
          }
        }
        break;

      default:
        //No default right now
        break;
    }

    usleep(100);
  }

  printf("Engine is dead!\n");
}


void Game::StartEngine()
{
  EngineRequest = 1;

  while (!EngineStatus)
    usleep(100);
}


void Game::StartEngineAsync()
{
  EngineRequest = 1;
}


void Game::StopEngine()
{
  EngineRequest = 0;

  while (EngineStatus)
    usleep(100);
}



void Game::StopEngineAsync()
{
  EngineRequest = 0;
}



void Game::EngineMakeMove()
{
  EngineRequest = ENGINEACTION_MOVE;
}



