#include "chess.h"

///////////////////
// TESTLEN = 1 --> Quick test (6 seconds max per test)
// TESTLEN = 2 --> Medium test (2 minutes max per test)
// TESTLEN = 3 --> Long (Overnight) test
///////////////////
#define TESTLEN 1


int VerifyHighEndMoveScore(int ply, int ExpectedScore, long long int TotalMoves, ResetTree *Root, int ExpectedTime)
{
  long long int ABIPMoveCount = 0;
  long long int ABLNSBMoveCount = 0;
  long long int PABLNSBMoveCount = 0;
  int retcode = 0;
  Timer ABIPTimer, ABLNSBTimer, PABLNSBTimer;
  int ABIPTime, ABLNSBTime, PABLNSBTime;
  int ABIPScore, ABLNSBScore, PABLNSBScore;
  int i;

  if ((ExpectedTime > 6) && (TESTLEN < 2))
    return(0);
  if ((ExpectedTime > 120) && (TESTLEN < 3))
    return(0);
  
  Root->InitializeMoveGeneration();
  ABIPTimer.Reset();
  ABIPScore = Root->IterativeAlphaBetaInPlace(ply,5,&ABIPMoveCount);
  ABIPTime = ABIPTimer.ElapsedTime();
  printf("Ply=%d, IterativeAB In Place[5]: %d (savings=%2.3f\%), Score=%d, Time=%d", ply, ABIPMoveCount,((float) (TotalMoves-ABIPMoveCount) * 100.0) / ((float) TotalMoves),ABIPScore,ABIPTime);
  Root->DeleteTree(RUNS_SERIALLY);

  if (abs(ABIPScore - ExpectedScore) > 1)
  {
    printf(" Expected %d, got %d (%d) ",ExpectedScore,ABIPScore,abs(ABIPScore - ExpectedScore));
    printf(" ERROR!\n");
    retcode = 1;
  }
  else
  {
    printf("\n");
  }

  Root->InitializeMoveGeneration();
  ABLNSBTimer.Reset();
  ABLNSBScore = Root->IterativeAlphaBetaLeaveNodesSaveBest(ply,5,0,0,&ABLNSBMoveCount);
  ABLNSBTime = ABLNSBTimer.ElapsedTime();
  printf("Ply=%d, IterativeABSaveBest LeaveNodes[5]: %d (savings=%2.3f\%), Score=%d, Time=%d", ply, ABLNSBMoveCount,((float) (TotalMoves-ABLNSBMoveCount) * 100.0) / ((float) TotalMoves),ABLNSBScore,ABLNSBTime);
  Root->DeleteTree(RUNS_SERIALLY);

  if (abs(ABLNSBScore - ExpectedScore) > 1)
  {
    printf(" Expected %d, got %d (%d) ",ExpectedScore,ABLNSBScore,abs(ABLNSBScore - ExpectedScore));
    printf(" ERROR!\n");
    retcode = 1;
  }
  else
  {
    printf("\n");
  }
  
  
  printf("\n");
  return(retcode);
}



int Game::VerifyIndividualMoveScore(char SearchRoutineText[], int ExpectedScore, long long int TotalMoves)
{
  Timer myTimer;
  int myTime;
  int retcode = 0;

  //Reset to run off Initial move (as set by FEN)
  Ply = 0; 
  MoveCount[Ply] = 0;
  CurrentBoard = InitialBoard;
  CurrentBoard->DeleteTree(RUNS_SERIALLY);
  CurrentBoard->InitializeMoveGeneration();
  printf("Depth=%d, %20s: ", ABParmlist.Depth, SearchRoutineText);
  myTimer.Reset();
  LaunchAsyncMoveSearch();
  WaitForMoveSearchToEnd();
  myTime = myTimer.ElapsedTime();

  printf("%d (savings=%2.3f\%), Score=%d, Time=%d", MoveCount[0],((float) (TotalMoves-MoveCount[0]) * 100.0) / ((float) TotalMoves),SearchScore[0],myTime);

  if (abs(SearchScore[0] - ExpectedScore) > 1)
  {
    printf(" Expected %d, got %d ",ExpectedScore,SearchScore[0]);
    printf(" ERROR!\n");
    retcode = 1;
  }
  else
  {
    printf("\n");
  }
  return(retcode);
}


int Game::VerifyHighEndMoveScore(int ply, int ExpectedScore, long long int TotalMoves, int ExpectedTime)
{
  int retcode = 0;
  char SearchRoutineText[100];

  if ((ExpectedTime > 6) && (TESTLEN < 2))
    return(0);
  if ((ExpectedTime > 120) && (TESTLEN < 3))
    return(0);

  //Set search parameters
  SearchRoutine = &Game::ParallelIterativeAlphaBetaLeaveNodesSaveBest;
  strcpy(SearchRoutineText,"Par Iter AB LN SB");
  NumThreads = 3;
  ABParmlist.Depth = ply;
  ABParmlist.LeaveDepth = 5;
  retcode += VerifyIndividualMoveScore(SearchRoutineText,ExpectedScore,TotalMoves);

  return(retcode);
}



int Game::VerifyMoveScore(int ply, int ExpectedScore, long long int TotalMoves, int ExpectedTime)
{
  int retcode = 0;
  char SearchRoutineText[100];

  if ((ExpectedTime > 6) && (TESTLEN < 2))
    return(0);
  if ((ExpectedTime > 120) && (TESTLEN < 3))
    return(0);

  //Set search parameters
  SearchRoutine = &Game::SerialAlphaBeta;
  strcpy(SearchRoutineText,"Serial AB");
  NumThreads = 1;
  ABParmlist.Depth = ply;
  retcode += VerifyIndividualMoveScore(SearchRoutineText,ExpectedScore,TotalMoves);

  //Set search parameters
  SearchRoutine = &Game::IterativeAlphaBeta;
  strcpy(SearchRoutineText,"Iterative AB");
  NumThreads = 1;
  ABParmlist.Depth = ply;
  ABParmlist.PrimeDepth = 5;
  retcode += VerifyIndividualMoveScore(SearchRoutineText,ExpectedScore,TotalMoves);

  retcode += VerifyHighEndMoveScore(ply,ExpectedScore,TotalMoves,ExpectedTime);

  return(retcode);
}


#ifdef COMMENT
int VerifyMoveScore(int ply, int ExpectedScore, long long int TotalMoves, ResetTree *Root, int ExpectedTime)
{
  long long int ABMoveCount = 0;
  long long int IABMoveCount[5] = {0,0,0,0,0};
  long long int ABIPMoveCount = 0;
  long long int ABSBMoveCount = 0;
  long long int ABLNSBMoveCount = 0;
  int retcode = 0;
  Timer SerialTimer, ABTimer, IABTimer, ABIPTimer, ABSBTimer, ABLNSBTimer;
  int SerialTime, ABTime, IABTime, ABIPTime, ABSBTime, ABLNSBTime;
  int IABScore[5], ABScore, ABIPScore, ABSBScore, ABLNSBScore;
  int i;

  if ((ExpectedTime > 6) && (TESTLEN < 2))
    return(0);
  if ((ExpectedTime > 120) && (TESTLEN < 3))
    return(0);
  
  Root->InitializeMoveGeneration();
  ABTimer.Reset();
  ABScore = Root->SerialAlphaBeta(ply,0,MAX_SCORE,MIN_SCORE,&ABMoveCount);
  ABTime = ABTimer.ElapsedTime();
  printf("Ply=%d, AB: %d (savings=%2.3f\%), Score=%d, Time=%d", ply, ABMoveCount,((float) (TotalMoves-ABMoveCount) * 100.0) / ((float) TotalMoves),ABScore,ABTime);

  if (abs(ABScore - ExpectedScore) > 1)
  {
    printf(" Expected %d, got %d (%d) ",ExpectedScore,ABScore,abs(ABScore - ExpectedScore));
    printf(" ERROR!\n");
    retcode = 1;
  }
  else
  {
    printf("\n");
  }

  Root->InitializeMoveGeneration();
  ABSBTimer.Reset();
  ABSBScore = Root->IterativeAlphaBetaSaveBest(ply,&ABSBMoveCount);
  ABSBTime = ABSBTimer.ElapsedTime();
  printf("Ply=%d, AB Save Best: %d (savings=%2.3f\%), Score=%d, Time=%d", ply, ABSBMoveCount,((float) (TotalMoves-ABSBMoveCount) * 100.0) / ((float) TotalMoves),ABSBScore,ABSBTime);
  Root->DeleteTree(RUNS_SERIALLY);

  if (abs(ABSBScore - ExpectedScore) > 1)
  {
    printf(" Expected %d, got %d (%d) ",ExpectedScore,ABSBScore,abs(ABSBScore - ExpectedScore));
    printf(" ERROR!\n");
    retcode = 1;
  }
  else
  {
    printf("\n");
  }

  for (i=2;i<3;i++)
  {
    Root->InitializeMoveGeneration();
    IABTimer.Reset();
    IABScore[i] = Root->IterativeAlphaBeta(i+1,ply,&IABMoveCount[i]);
    IABTime = IABTimer.ElapsedTime();
    printf("Ply=%d, IterativeAB(%d): %d (savings=%2.3f\%), Score=%d, Time=%d",ply,i+1,IABMoveCount[i],((float) (TotalMoves-IABMoveCount[i]) * 100.0) / ((float) TotalMoves),IABScore[i],IABTime);
  
    if (abs(IABScore[i] - ExpectedScore) > 1)
    {
      printf(" Expected %d, got %d (%d) ",ExpectedScore,IABScore[i],abs(IABScore[i] - ExpectedScore));
      printf(" ERROR!\n");
      retcode = 1;
    }
    else
    {
      printf("\n");
    }
  }

  retcode += VerifyHighEndMoveScore(ply, ExpectedScore, TotalMoves, Root, ExpectedTime);
  return(retcode);
}
#endif


int VerifyMoveGeneration(int ply, long long int Expected, ResetTree *Root, int ExpectedTime)
{
  long long int ABMoveCount = 0;
  long long int IABMoveCount[5] = {0,0,0,0,0};
  long long int SerialMoveCount = 0;
  int retcode = 0;
  Timer SerialTimer, ABTimer, IABTimer;
  int SerialTime, ABTime, IABTime;
  int IABScore[5], ABScore, SerialScore;
  int i;

  if ((ExpectedTime > 6) && (TESTLEN < 2))
    return(0);
  if ((ExpectedTime > 120) && (TESTLEN < 3))
    return(0);
  
  Root->InitializeMoveGeneration();
  printf("Ply=%d, SERIAL: ",ply);
  SerialTimer.Reset();
  SerialScore = Root->SerialMoveTree(ply,0,&SerialMoveCount);
  SerialTime = SerialTimer.ElapsedTime();
  printf("Expected=%d, Actual=%d, Score=%d, Time=%d",Expected,SerialMoveCount,SerialScore,SerialTime);
  if (SerialMoveCount != Expected)
  {
    printf(" ERROR!\n");
    retcode = 1;
  }
  else
  {
    printf("\n");
  }
  return retcode;
}


int CompareABvsNaive(int ply, ResetTree *Root)
{
  long long int ABMoveCount = 0;
  long long int SerialMoveCount = 0;
  int retcode = 0;
  Timer myTimer;
  int ABScore, SerialScore;

  Root->InitializeMoveGeneration();
  myTimer.Reset();
  ABScore = Root->SerialAlphaBeta(ply,0,MAX_SCORE,MIN_SCORE,&ABMoveCount);

  Root->InitializeMoveGeneration();
  myTimer.Reset();
  SerialScore = Root->SerialMoveTree(ply,0,&SerialMoveCount);

  printf("ABMovesSaved: %d (%3.0f), ",SerialMoveCount-ABMoveCount,(float) ABMoveCount / ((float) SerialMoveCount));
  if (ABScore != SerialScore)
  {
    printf(" ERROR!\n");
    retcode = 1;
  }
  else
  {
    printf("\n");
  }
  return(retcode);
}


int TestMoves()
{
  //Game & MyGame = *new Game;
  Game MyGame;
  ResetTree & Root = *new ResetTree;
  long long int MoveCount;
  long long int Expected;
  int retcode = 0;

  //Turn off output buffering.
  setbuf(stdout, NULL);

  printf("##########################################################\n");
  printf("##########################################################\n");
  printf("##################  Running Test Suite  ##################\n");
  printf("##########################################################\n");
  printf("##########################################################\n\n");

  //Root.bInitToFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  //retcode += VerifyMoveCount(6,119060324,&Root,18);
//return(0);

  printf("==> Initial Board\n");
  MyGame.SetGame("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  Root.bInitToFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  retcode += VerifyMoveGeneration(1,20,&Root,0);
  retcode += MyGame.VerifyMoveScore(1,946,20,0);
  retcode += VerifyMoveGeneration(2,400,&Root,0);
  retcode += MyGame.VerifyMoveScore(2,-601,400,0);
  retcode += VerifyMoveGeneration(3,8902,&Root,0);
  retcode += MyGame.VerifyMoveScore(3,836,8902,0);
  retcode += VerifyMoveGeneration(4,197281,&Root,0);
  retcode += MyGame.VerifyMoveScore(4,-797,197281,0);
  retcode += VerifyMoveGeneration(5,4865609,&Root,1);
  retcode += MyGame.VerifyMoveScore(5,999033,4865609,0);
  retcode += VerifyMoveGeneration(6,119060324,&Root,24);
  retcode += MyGame.VerifyMoveScore(6,-999025,119060324,1);
  retcode += VerifyMoveGeneration(7,3195901860,&Root,840);
  retcode += MyGame.VerifyMoveScore(7,999495,3195901860,4); //Hi:2
  //retcode += VerifyMoveGeneration(8,84998978956,&Root,10000); //too long
  retcode += MyGame.VerifyHighEndMoveScore(8,-999036,84998978956,16); //Hi:16 Lo:69
  //retcode += VerifyMoveGeneration(9,2439530234167,&Root,10000); //too long
  retcode += MyGame.VerifyHighEndMoveScore(9,1000386,2439530234167,53); //Hi:53 Lo:303
  //retcode += VerifyMoveGeneration(10,69352859712417,&Root,10000); //too long
  retcode += MyGame.VerifyHighEndMoveScore(10,-999067,69352859712417,601); //Hi:601 Lo:6334
  printf("\n");
return(retcode);
  printf("==> Bishop Testing\n");
  Root.bInitToFEN("k7/8/8/8/8/8/8/B6K w - - 0 1");
  retcode += VerifyMoveGeneration(1,10,&Root,0);
  retcode += MyGame.VerifyMoveScore(1,987,10,0);
  retcode += VerifyMoveGeneration(2,28,&Root,0);
  retcode += MyGame.VerifyMoveScore(2,197,28,0);
  retcode += VerifyMoveGeneration(3,367,&Root,0);
  retcode += MyGame.VerifyMoveScore(3,822,367,0);
  printf("\n");

  printf("==> Midgame Test 1\n");
  Root.bInitToFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
  Root.TestSafety(WHITE,0xfff54e148a802200);
  Root.TestSafety(BLACK,0x81600051a850ebff);
  retcode += VerifyMoveGeneration(1,48,&Root,0);
  retcode += MyGame.VerifyMoveScore(1,3000022,48,0);
  retcode += VerifyMoveGeneration(2,2039,&Root,0);
  retcode += MyGame.VerifyMoveScore(2,812,2039,0);
  retcode += VerifyMoveGeneration(3,97862,&Root,0);
  retcode += MyGame.VerifyMoveScore(3,2999473,97862,0);
  retcode += VerifyMoveGeneration(4,4085603,&Root,0);
  retcode += MyGame.VerifyMoveScore(4,380,4085603,0);
  retcode += VerifyMoveGeneration(5,193690690,&Root,43);
  retcode += MyGame.VerifyMoveScore(5,2000510,193690690,0); 
  retcode += VerifyMoveGeneration(6,8031647685,&Root,1370); 
  retcode += MyGame.VerifyMoveScore(6,-999874,8031647685,3); //iterative3=1
  printf("\n");

  printf("==> Midgame Test 2\n");
  Root.bInitToFEN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
  retcode += VerifyMoveGeneration(1,6,&Root,0);
  retcode += MyGame.VerifyMoveScore(1,711,6,0);
  retcode += VerifyMoveGeneration(2,264,&Root,0);
  retcode += MyGame.VerifyMoveScore(2,-4999029,264,0);
  retcode += VerifyMoveGeneration(3,9467,&Root,0);
  retcode += MyGame.VerifyMoveScore(3,371,9467,0);
  retcode += VerifyMoveGeneration(4,422333,&Root,0);
  retcode += MyGame.VerifyMoveScore(4,-5000411,422333,0);
  retcode += VerifyMoveGeneration(5,15833292,&Root,3);
  retcode += MyGame.VerifyMoveScore(5,-2000745,15833292,0);
  retcode += VerifyMoveGeneration(6,706045033,&Root,134); 
  retcode += MyGame.VerifyMoveScore(6,-5999843,706045033,0); //Guess
  printf("\n");

  printf("==> Promotion Test 1\n");
  Root.bInitToFEN("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
  retcode += VerifyMoveGeneration(1,24,&Root,0);
  retcode += MyGame.VerifyMoveScore(1,-2999923,24,0);
  retcode += VerifyMoveGeneration(2,496,&Root,0);
  retcode += MyGame.VerifyMoveScore(2,-520,496,0);
  retcode += VerifyMoveGeneration(3,9483,&Root,0);
  retcode += MyGame.VerifyMoveScore(3,-2999465,9483,0);
  retcode += VerifyMoveGeneration(4,182838,&Root,0);
  retcode += MyGame.VerifyMoveScore(4,-1999033,182838,0);
  retcode += VerifyMoveGeneration(5,3605103,&Root,3);
  retcode += MyGame.VerifyMoveScore(5,-3000123,3605103,0);
  retcode += VerifyMoveGeneration(6,71179139,&Root,22);
  retcode += MyGame.VerifyMoveScore(6,-1999857,71179139,0); //Guess
  printf("\n");

  printf("==> Endgame Test 1\n");
  Root.bInitToFEN("4k3/3r4/4K3/7R/8/8/8/8 w - - 0 1");
  retcode += VerifyMoveGeneration(1,17,&Root,0);
  retcode += MyGame.VerifyMoveScore(1,101000000,17,0);
  printf("\n");

  printf("==> Endgame Test 2\n");
  Root.bInitToFEN("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
  retcode += VerifyMoveGeneration(1,14,&Root,0);
  retcode += MyGame.VerifyMoveScore(1,999717,14,0);
  retcode += VerifyMoveGeneration(2,191,&Root,0);
  retcode += MyGame.VerifyMoveScore(2,999132,191,0);
  retcode += VerifyMoveGeneration(3,2812,&Root,0);
  retcode += MyGame.VerifyMoveScore(3,1000798,2812,0);
  retcode += VerifyMoveGeneration(4,43238,&Root,0);
  retcode += MyGame.VerifyMoveScore(4,-68,43238,0);
  retcode += VerifyMoveGeneration(5,674624,&Root,0);
  retcode += MyGame.VerifyMoveScore(5,1000590,674624,0);
  retcode += VerifyMoveGeneration(6,11030083,&Root,3);
  retcode += MyGame.VerifyMoveScore(6,171,11030083,5);
  retcode += VerifyMoveGeneration(7,178633661,&Root,48);
  retcode += MyGame.VerifyMoveScore(7,1000668,178633661,0); //Guess
  printf("\n");

  printf("==> Castle Testing\n");
  Root.bInitToFEN("3rk3/8/8/8/8/8/8/R3K2R w KQ - 0 1");
  retcode += VerifyMoveGeneration(1,23,&Root,0);
  Root.bInitToFEN("4kr2/8/8/8/8/8/8/R3K2R w KQ - 0 1");
  retcode += VerifyMoveGeneration(1,23,&Root,0);
  Root.bInitToFEN("4k3/8/b7/8/8/8/8/R3K2R w KQ - 0 1");
  retcode += VerifyMoveGeneration(1,21,&Root,0);
  Root.bInitToFEN("4k3/8/8/8/8/8/4p3/R3K2R w KQ - 0 1");
  retcode += VerifyMoveGeneration(1,22,&Root,0);
  Root.bInitToFEN("3k4/b1p5/1pP5/pP5p/P5pP/6P1/8/R3K3 w Q - 0 1");
  retcode += VerifyMoveGeneration(1,11,&Root,0);
  retcode += VerifyMoveGeneration(2,42,&Root,0);
  Root.bInitToFEN("5k2/b1p1p1p1/1pP1P1P1/pP6/P6p/7P/8/4K2R w K - 0 1");
  retcode += VerifyMoveGeneration(1,9,&Root,0);
  retcode += VerifyMoveGeneration(2,25,&Root,0);
  Root.bInitToFEN("4k2r/3p3p/3P3P/8/8/6p1/6PB/5K2 b k - 0 1");
  retcode += VerifyMoveGeneration(1,7,&Root,0);
  retcode += VerifyMoveGeneration(2,31,&Root,0);
  Root.bInitToFEN("r3k3/p3p3/P3P3/8/8/1Pp5/2P5/3K4 b q - 0 1");
  retcode += VerifyMoveGeneration(1,6,&Root,0);
  retcode += VerifyMoveGeneration(2,22,&Root,0);
  printf("\n");

  printf("==> Not sure what to call this\n");
  Root.bInitToFEN("8/3K4/2p5/p2b2r1/5k2/8/8/1q6 b - 1 67");
  retcode += VerifyMoveGeneration(1,50,&Root,0);
  retcode += VerifyMoveGeneration(2,279,&Root,0);
  printf("\n");

  printf("==> Not sure what to call this 2\n");
  Root.bInitToFEN("8/7p/p5pb/4k3/P1pPn3/8/P5PP/1rB2RK1 b - d3 0 28");
  retcode += VerifyMoveGeneration(1,5,&Root,0); 
  retcode += VerifyMoveGeneration(2,117,&Root,0); 
  retcode += VerifyMoveGeneration(3,3293,&Root,0); 
  retcode += VerifyMoveGeneration(4,67197,&Root,0); 
  retcode += VerifyMoveGeneration(5,1881089,&Root,0); 
  retcode += VerifyMoveGeneration(6,38633283,&Root,11); 
  printf("\n");

  printf("==> Not sure what to call this 3\n");
  Root.bInitToFEN("rnbqkb1r/ppppp1pp/7n/4Pp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3");
  retcode += VerifyMoveGeneration(1,31,&Root,0);
  retcode += VerifyMoveGeneration(2,570,&Root,0);
  retcode += VerifyMoveGeneration(3,17546,&Root,0);
  retcode += VerifyMoveGeneration(4,351806,&Root,0);
  retcode += VerifyMoveGeneration(5,11139762,&Root,2);
  printf("\n");

  printf("==> Not sure what to call this 4\n");
  Root.bInitToFEN("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
  retcode += VerifyMoveGeneration(1,14,&Root,0);
  retcode += VerifyMoveGeneration(2,191,&Root,0);
  retcode += VerifyMoveGeneration(3,2812,&Root,0);
  retcode += VerifyMoveGeneration(4,43238,&Root,0);
  retcode += VerifyMoveGeneration(5,674624,&Root,0); 
  retcode += VerifyMoveGeneration(6,11030083,&Root,3); 
  retcode += VerifyMoveGeneration(7,178633661,&Root,48);
  printf("\n");

  printf("==> En Passant Testing \n");
  Root.bInitToFEN("4k3/8/8/2PpP3/8/8/8/4K3 w - d6 0 1");
  retcode += VerifyMoveGeneration(1,9,&Root,0);
  Root.bInitToFEN("4k3/8/8/pP6/8/8/8/4K3 w - a6 0 1");
  retcode += VerifyMoveGeneration(1,7,&Root,0);
  Root.bInitToFEN("k7/ppp5/8/1P6/8/8/8/4K3 b - - 0 1");
  retcode += VerifyMoveGeneration(1,6,&Root,0);
  retcode += VerifyMoveGeneration(2,39,&Root,0);
  Root.bInitToFEN("k7/1p1p4/1PbP4/1p6/1P1p3p/7P/4P3/6K1 w - - 0 1");
  retcode += VerifyMoveGeneration(1,5,&Root,0);
  retcode += VerifyMoveGeneration(2,34,&Root,0);
  printf("\n");

  printf("==> Mate in 6\n");
  Root.bInitToFEN("r1br4/1p2npkp/3Bpbp1/pqp5/2N1R3/1P1P1QP1/1PP2PBP/R5K1 w - - 0 1");
  retcode += VerifyMoveGeneration(1,49,&Root,0);
  retcode += VerifyMoveGeneration(2,1885,&Root,0);
  retcode += VerifyMoveGeneration(3,91601,&Root,0);
  retcode += VerifyMoveGeneration(4,3444984,&Root,1);
  retcode += VerifyMoveGeneration(5,165891050,&Root,40); 
  retcode += VerifyMoveGeneration(6,1,&Root,1089); 
  //retcode += VerifyMoveGeneration(7,1,&Root,73815); 
  //retcode += VerifyMoveGeneration(8,39,&Root);
  printf("\n");

  if (retcode)
  {
    printf("##########################################################\n");
    printf("##########################################################\n");
    printf("##################        ERROR!        ##################\n");
    printf("##########################################################\n");
    printf("##########################################################\n");
    return (1);
  }
  else
  {
    printf("\nNO ERRORS DETECTED\n");
    return (0);
  }
}

