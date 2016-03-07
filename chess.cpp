#include "chess.h"

//#define TESTMODE

Game MyGame;
FILE *logfile;

int main()
{
  char temp;
  ResetTree & Root = *new ResetTree;
  
  logfile = fopen("/home/jrotter/git/chessica/logs/chessica.log","w");
  InitKingStars();
  InitAllAttacks();
  Root.InitCheckFunctionRouters();

  Root.InitFreeList();
  //Root.bInitToFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  //Root.PrintReset();

#ifdef TESTMODE
  TestMoves();
#else
  MyGame.XboardCommandInterpreter();
#endif

  fclose(logfile);
  return (0);
}
