#include <iostream>
#include <pthread.h>
using namespace std;	// Needed for calling cout, endl, etc.

class Game {

/*************************************************************/
/*************************************************************/
/************************ PRIVATE ****************************/
/*************************************************************/
/*************************************************************/
private:

  typedef struct {
    int Depth;
    int MaxHold;
    int LeaveDepth;
    int PrimeDepth;
  } ABParms; //Note that this definition is duplicated in chess.h

  typedef struct {
    Game *ThisGame;
    int *Retvalue;
  } EngineParms;

  typedef int (Game::*SearchRoutineFunction)(int ThreadID);
  
  //static variables
  int EngineRequest; //0=stop, 1=go
  int EngineStatus; //0=stopped, 1=going
  int ProgramActive;  
  char UserMove[10];

  ResetTree *InitialBoard;
  ResetTree *CurrentBoard;
  int WhitePlayer;
  int BlackPlayer;
  int Ply;
  int MoveNumber;
  int XBoard; //1 if xboard called the engine, 0 otherwise
  int Ponder; //1 to Ponder
  int ShowThinking;

  //Multithreading
  typedef struct {
    Game *myGame;
    int inThreadID;
  } SearchThreadParms;  //also defined in chess.h

  typedef struct {
    long long int MoveCount;
    pthread_t SearchThread;
    ResetTree *Root;
    int Nodes; //Number of children this thread owns
    int Score; //Best score found by this thread
    SearchThreadParms ThreadParms;
  } ThreadDataType;

  ThreadDataType ThreadData[SEARCH_THREADS];
  int NumThreads;
  SearchRoutineFunction SearchRoutine;
  int TotalNodes;
  Lock ThreadLock;
  int SyncCounter;

  ABParms ABParmlist;
  EngineParms EngineParmList;
  pthread_t EngineThread;
  ResetTree *DebugPtr;
  int SearchScore[MAX_MOVES_PER_GAME];
  long long int MoveCount[MAX_MOVES_PER_GAME];

  //game.cpp
  int ComputerToMove(); 

  void InitIO();	//xboard.cpp
  void ScheduleComputerMove(int seconds);	//xboard.cpp

  //engine.cpp
  void MakeMove(); 
  int Engine();
  static void * EngineStatic(void *);

  //ab.cpp
  int IterativeAlphaBetaSaveBest(int ThreadID);
  int ParallelIterativeAlphaBetaLeaveNodesSaveBest(int ThreadID);

  //search.cpp
  static void * SearchThreadLauncher(void * parms);
  void SearchThreadRoot(int ThreadID);
  void LaunchAsyncMoveSearch();	//xboard.cpp
  void LaunchAsyncMoveSearch2();	//xboard.cpp
  void HaltAsyncMoveSearch();	//xboard.cpp
  void WaitForMoveSearchToEnd();
  
  //test_ab.cpp
  int SerialAlphaBeta(int ThreadID);
  int IterativeAlphaBeta(int ThreadID);
  int IterativeAlphaBetaInPlace(int ThreadID);


/*************************************************************/
/*************************************************************/
/************************* PUBLIC ****************************/
/*************************************************************/
/*************************************************************/
public:

  static void ComputerMoveInterruptHandler(int x);	//xboard.cpp

  //game.cpp
  Game();	//constructor
  void StartGame(); 
  void InitGame(); 
  void SetGame(const char * FEN); 
  void InitXBoardGame(); 
  void GameCleanup(); 
  void PrimeSearchThreadTrees();
  void MergeSearchThreadTrees();

  //engine.cpp
  void LaunchEngine();
  void KillEngine();
  void StartEngine();
  void StartEngineAsync();
  void StopEngine();
  void StopEngineAsync();
  void EngineMakeMove();

  //xboard.cpp
  void XboardCommandInterpreter();

  //test_main.cpp
  int VerifyIndividualMoveScore(char SearchRoutineText[], int ExpectedScore, long long int TotalMoves);
  int VerifyMoveScore(int ply, int ExpectedScore, long long int TotalMoves, int ExpectedTime);
  int VerifyHighEndMoveScore(int ply, int ExpectedScore, long long int TotalMoves, int ExpectedTime);
};

  
