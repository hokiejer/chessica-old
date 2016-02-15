#include <iostream>
#include <pthread.h>
using namespace std;	// Needed for calling cout, endl, etc.

class ResetTree : public Reset
{
/*************************************************************/
/*************************************************************/
/************************ PRIVATE ****************************/
/*************************************************************/
/*************************************************************/
private:

  typedef struct ResetList_struct {
    ResetTree *First;
    ResetTree *Last;
    pthread_mutex_t Mutex;
  } ResetList;

  //static variables
  static int CeaseProcessing;
  static ResetTree *FreeList[MAX_THREADS];


  ResetTree *PrevSibling;
  ResetTree *NextSibling;
  ResetTree *Parent;
  ResetList Children;
  unsigned BestMoveSaved :1;	// Last move generation saved only best move
  unsigned Reserved01 :7;

  void PopulateFreeList(int ThreadID);	//tree.cpp
  void InitHashTable();		//hash.cpp
  void InitializeSerialization();	//tree.cpp
  void Serialize();		//tree.cpp
  int Serialize_Or_Skip();		//tree.cpp
  void Unserialize();		//tree.cpp
  ResetTree * FindMatchingChild(ResetTree *MyNode);	//tree.cpp
  ResetTree * FindMatchingChild(char fromsquare, char tosquare);  //tree.cpp


/*************************************************************/
/*************************************************************/
/************************* PUBLIC ****************************/
/*************************************************************/
/*************************************************************/
public:

  //tree.cpp
  ResetTree();	//constructor
  void InitFreeList();
  int CountFreeList();
  int CountChildren();
  int CountTree();
  ResetTree *GetNode(int ThreadID);
  ResetTree *NewChild(int ThreadID);
  ResetTree *NewOrphan(int ThreadID);
  void AddChild(ResetTree *Newguy);
  void DeleteMe(int ThreadID);
  void DeleteChildren(int ThreadID);
  void DeleteTree(int ThreadID);
  void DeleteFromSiblingList(ResetTree **PointerToThisNode, int ThreadID);
  void DeleteChild(ResetTree *MyChild, int ThreadID);
  ResetTree *DequeueChild(ResetTree *MyChild);
  ResetTree *DequeueChildNotFirst(ResetTree *MyChild);
  ResetTree *DequeueChildNotLast(ResetTree *MyChild);
  ResetTree *DequeueFirstChild();
  void PutNodeFirstInList(ResetTree *Newguy);
  void PutNodeLastInList(ResetTree *Newguy);
  void GetMoveText(char buffer[]);
  static void *MoveTreeStatic(void *);
  void TreeDiagnostics();
  void PrepareToSearch();
  int AbortTreeSearch();
  int StillSearching();

  //moves.cpp
  int MoveTree(int Depth, int Ply, long long int *NodeCount, int ThreadID);
  int ParallelMoveTree(int Depth, int Ply, long long int *NodeCount);
  int SerialMoveTree(int Depth, int Ply, long long int *NodeCount);
  void GenerateMoves(int Depth, int ThreadID);

  //movetree.cpp
  void PruneAllButFirst(int ThreadID);
  ResetTree *UseBestMove();
  ResetTree *UseThisMove(char fromsquare, char tosquare, int promotion);
  int MoveIsValid(char fromsquare, char tosquare, int promotion, int ThreadID);
  ResetTree *ConsoleDebug(char []);
  void PrintMoveOptions();

  //test_ab.cpp
  int SerialAlphaBeta(int Depth, int Ply, int Min, int Max, long long int *NodeCount);
  int SerialAlphaBetaOnList(int Depth, int Ply, int Min, int Max, long long int *NodeCount);
  int IterativeAlphaBeta(int PrimeDepth, int Depth, long long int *NodeCount);
  int AlphaBetaCapturesOnly(int Min, int Max, long long int *NodeCount);
  int AlphaBetaInPlace(int MaxHold, int Depth, int Ply, int Min, int Max, long long int *NodeCount);
  int IterativeAlphaBetaInPlace(int Depth, int MaxHold, long long int *NodeCount);
  static void * IterativeABIPStatic(void * parms);
  int Profiling_ParallelABLNSB(int Depth, int LeaveDepth, long long int *MoveCount);

  //ab.cpp
  int IterativeAlphaBetaSaveBest(int Depth, long long int *NodeCount);
  int IterativeAlphaBetaLeaveNodesSaveBest(int Depth, int LeaveDepth, int ShowThinking, int ThreadID, long long int *NodeCount);
  int ParallelIterativeAlphaBetaLeaveNodesSaveBest(int Depth, int LeaveDepth, int ShowThinking, int ThreadID, Lock *ThreadLock, int *ThreadStatus, long long int *NodeCount);
  static void * IterativeABLNSBStatic(void * parms);
  static void * ParallelIterativeABLNSBStatic(void * parms);
  int AlphaBetaLeaveNodesSaveBest(int LeaveDepth, int Depth, int Ply, int Min, int Max, int ThreadID, long long int *NodeCount);
  int AlphaBetaSaveBest(int Depth, int Ply, int Min, int Max, int ThreadID, long long int *NodeCount);

  //score.cpp
  int ScoreResetNode();

  //print.cpp
  void PrintChildren();
  void PrintBestMoveLine();
  char *PrintBestMoveLineToBuffer(char *outbuffer);

};

  
