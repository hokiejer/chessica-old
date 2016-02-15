#include "chess.h"
#include <signal.h>

void Game::InitIO()
{
  //Turn off input/output buffering.  Needed for xboard interface
  setbuf(stdout, NULL);
  setbuf(stdin, NULL);
  
  //ignore SIGINT and SIGTERM
  //signal(SIGINT, SIG_IGN);
  signal(SIGTERM, SIG_IGN);
}


void Game::ComputerMoveInterruptHandler(int x)
{
  MyGame.EngineMakeMove();
}


void Game::ScheduleComputerMove(int seconds)
{
  signal(SIGALRM,ComputerMoveInterruptHandler);
  alarm(seconds);
}

void Game::XboardCommandInterpreter()
{
  char args[10][100];
  char mybuffer[100];
  char fromtext[2], totext[2];
  char fromsquare, tosquare;
  char filename[] = "/home/jrotter/git/chessica/logs/xboardlog.txt";
  ResetTree *TempBoard;
  int promotion = 0;
  int i;

  char logstring[200];


  InitIO();
  InitGame();
  LaunchEngine();

  while (1)
  {
    scanf("%s",args[0]);
    sprintf(logstring,"[xboard] %s\n",args[0]);
    Log.Write(logstring);

    //xboard saying hello
    if (strcmp(args[0],"xboard") == 0)
    {
      IOLock.Obtain();
      printf("\n");
      IOLock.Release();
    }

    //xboard indicating protocol version
    if (strcmp(args[0],"protover") == 0)
    {
      scanf("%s",args[1]); //get version
      //printf("# protover=%s\n",args[1]);
      //if (strcmp(args[1],"4") == 0)
      //{
        //This better be true because I only speak in protocol 4
        IOLock.Obtain();
        printf("feature done=0\n"); //I'm not done yet
        Log.Write("feature done=0\n");
        printf("feature debug=1\n"); //Let me send comments to xboard
        Log.Write("feature debug=1\n"); 
        printf("feature setboard=1\n"); //Let xboard set positions with FEN
        Log.Write("feature setboard=1\n");
        printf("feature colors=0\n"); //Disable colors command
        Log.Write("feature colors=0\n");
        printf("feature myname=\"Chessica\"\n");
        Log.Write("feature myname=\"Chessica\"\n");
        printf("feature sigterm=0\n");
        Log.Write("feature sigterm=0\n");
        printf("feature sigint=0\n");
        Log.Write("feature sigint=0\n");
        printf("feature done=1\n"); //Now I'm done
        Log.Write("feature done=1\n");
        IOLock.Release();
      //}

      //This is code only run when xboard is involved
      InitXBoardGame();
    }

    //xboard indicating that it was OK with feature requests
    //NOTE: These come after each feature command, not in summaray
    //setboard
    //accepted
    if (strcmp(args[0],"accepted") == 0)
    {
      IOLock.Obtain();
      printf("# ACCEPTED: hooray!\n");
      Log.Write("# ACCEPTED: hooray!\n");
      IOLock.Release();
    }
    if (strcmp(args[0],"rejected") == 0)
    {
      IOLock.Obtain();
      printf("# REJECTED: BOOOOO!\n");
      Log.Write("# REJECTED: BOOOOO!\n");
      IOLock.Release();
    }


    //xboard asking for new game
    if (strcmp(args[0],"new") == 0)
    {
      InitGame();
      WhitePlayer = HUMAN_PLAYER; //This is a hack for now
      BlackPlayer = COMPUTER_PLAYER; //This is a hack for now
      if (Ponder || ComputerToMove())
        StartEngineAsync(); //ponder
    }


    //xboard asking to quit
    if (strcmp(args[0],"quit") == 0)
    {
      StopEngine();
      KillEngine();
      //GameCleanup(); $$$ I really need to clean up all of my allocated stuff
      //               $$$ This means the game tree and the freelist
      Log.Close();
      exit(0);
    }


    //xboard setting both players to human
    if (strcmp(args[0],"setboard") == 0)
    {
      mybuffer[0] = '\0';
      for (i=0;i<6;i++)
      {
        scanf("%s",args[0]);
        sprintf(logstring,"[xboard] %s\n",args[0]);
        Log.Write(logstring);
        strcat(mybuffer,args[0]);
      }
      SetGame(mybuffer); //Set the game to the new FEN
      StartEngineAsync(); //ponder
      
      //I have written this routine with no regard for any previous game activity
    }


    //xboard setting both players to human
    if (strcmp(args[0],"easy") == 0)
    {
      Ponder = 0;
      StopEngine();
    }


    //xboard setting both players to human
    if (strcmp(args[0],"hard") == 0)
    {
      Ponder = 1;
      StartEngineAsync();
    }


    //xboard setting both players to human
    if (strcmp(args[0],"force") == 0)
    {
      StopEngine();
      WhitePlayer = HUMAN_PLAYER; //This is a hack for now
      BlackPlayer = HUMAN_PLAYER; //This is a hack for now
    }


    //Leave force mode and set the engine to play the color that is on move
    //Start thinking and eventually make a move.
    if (strcmp(args[0],"go") == 0)
    {
      StopEngine(); //This always runs from Force Mode, but just in case
      if (CurrentBoard->WhiteToMove())
      {
        WhitePlayer = COMPUTER_PLAYER; 
        BlackPlayer = HUMAN_PLAYER; 
      }
      else
      {
        WhitePlayer = HUMAN_PLAYER;
        BlackPlayer = COMPUTER_PLAYER;
      }
      StartEngineAsync();
      ScheduleComputerMove(2);
    }


    //move sent from xboard
    if (
        (((args[0][0] == 'a') ||
          (args[0][0] == 'b') ||
          (args[0][0] == 'c') ||
          (args[0][0] == 'd') ||
          (args[0][0] == 'e') ||
          (args[0][0] == 'f') ||
          (args[0][0] == 'g') ||
          (args[0][0] == 'h')) &&
         ((args[0][1] == '1') ||
          (args[0][1] == '2') ||
          (args[0][1] == '3') ||
          (args[0][1] == '4') ||
          (args[0][1] == '5') ||
          (args[0][1] == '6') ||
          (args[0][1] == '7') ||
          (args[0][1] == '8')) &&
         ((args[0][2] == 'a') ||
          (args[0][2] == 'b') ||
          (args[0][2] == 'c') ||
          (args[0][2] == 'd') ||
          (args[0][2] == 'e') ||
          (args[0][2] == 'f') ||
          (args[0][2] == 'g') ||
          (args[0][2] == 'h')) &&
         ((args[0][3] == '1') ||
          (args[0][3] == '2') ||
          (args[0][3] == '3') ||
          (args[0][3] == '4') ||
          (args[0][3] == '5') ||
          (args[0][3] == '6') ||
          (args[0][3] == '7') ||
          (args[0][3] == '8'))) ||
        (strcmp(args[0],"O-O") == 0) ||
        (strcmp(args[0],"O-O-O") == 0) 
       )
    {
      if (args[0][0] == 'O') //Castle
      {
        if (strcmp(args[0],"O-O") == 0)
        {
          if (Ply % 2 == 0) //White
          {
            fromsquare = 4;
            tosquare = 6;
          }
          else
          {
            fromsquare = 60;
            tosquare = 62;
          }
        }
        if (strcmp(args[0],"O-O-O") == 0)
        {
          if (Ply % 2 == 0) //White
          {
            fromsquare = 4;
            tosquare = 2;
          }
          else
          {
            fromsquare = 60;
            tosquare = 58;
          }
        }
      }
      else //other move
      {
        fromtext[0] = args[0][0];
        fromtext[1] = args[0][1];
        fromsquare = TextToSquareNumber(fromtext);
        totext[0] = args[0][2];
        totext[1] = args[0][3];
        tosquare = TextToSquareNumber(totext);
        if (args[0][4] == 'n')
          promotion = 1;
        if (args[0][4] == 'b')
          promotion = 2;
        if (args[0][4] == 'r')
          promotion = 3;
        if (args[0][4] == 'q')
          promotion = 4;
      }
      
      if (CurrentBoard->MoveIsValid(fromsquare,tosquare,promotion,IO_THREAD))
      {
        strcpy(UserMove,args[0]);
        EngineMakeMove();
        ScheduleComputerMove(2);
      }
      else
      {
        //illegal move
        IOLock.Obtain();
        printf("Illegal move: %s\n",args[0]);
        sprintf(logstring,"Illegal move: %s\n",args[0]);
        Log.Write(logstring);
        IOLock.Release();
      }


    }
        
    //DEBUGGING
    if (strcmp(args[0],"ResetDebugPtr") == 0)
    {
      DebugPtr = CurrentBoard;
    }

    if ((strcmp(args[0],"help") == 0) ||
        (strcmp(args[0],"PrintReset") == 0) ||
        (strcmp(args[0],"PrintBoard") == 0) ||
        (strcmp(args[0],"PrintMoveOptions") == 0) ||
        (strcmp(args[0],"TreeDiags") == 0) ||
        (strcmp(args[0],"FirstChild") == 0) ||
        (strcmp(args[0],"NextChild") == 0) ||
        (strcmp(args[0],"PrevChild") == 0)
       )
    {
      DebugPtr = DebugPtr->ConsoleDebug(args[0]);
    }



  }
}

