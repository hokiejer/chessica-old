#include "chess.h"


int Reset::ScoreResetMaterialOnly()
{
  Reset TempChild, TempParent;
  Reset *LocalCopy = &TempParent;
  
  //Make a local copy so I don't screw up iterative AB
  CopyReset(LocalCopy);
  LocalCopy->InitMyChild(&TempChild);
    
  //Look for any children 
  LocalCopy->InitializeMoveGeneration();
  if (!LocalCopy->GenerateNextMove(&TempChild)) //If no children
  {
    GameOver = TRUE;
    if (ToMove == WHITE)
    {
      if (WhiteInCheck) //Checkmate: Black wins
      {
        return(Score = CheckMateScore[BLACK]);
      }
      else //Draw (this includes the 50 move rule)
      {
        return(Score = DrawScore[WHITE]);
      }
    }
    else //ToMove == BLACK
    {
      if (BlackInCheck) //Checkmate: White wins
      {
        return(Score = CheckMateScore[WHITE]);
      }
      else //Draw (this includes the 50 move rule)
      {
        return(Score = DrawScore[BLACK]);
      }
    }
  }

  Score = Material * 1000000;

  //Score += (0.001 * ((float) (HashValue/LARGE_PRIME) ));
  //Score += ((HashValue % LARGE_PRIME) / 100000);
  //Use 1997:
  Score += ((int) (bAll % 1997)) - 999; //3 Low order digits are "random"

  ScoreDepth = 0;

  return (Score);
}

//Pawns: Islands minimized (light weight)
//Pawns: Self protected?

//Bishops: Good Bishop, Bad Bishop
//Bishops: Mobility
//Bishops: Outpost - pawn protected

//Knights: Mobility
//Knights: Outpost - pawn protected


int ResetTree::ScoreResetNode()
{
  int MaterialScore;
  long long int MoveCount = 0;

  MaterialScore = ScoreResetMaterialOnly();
  //MaterialScore = AlphaBetaCapturesOnly(MAX_SCORE,MIN_SCORE,&MoveCount);
  //printf("MoveCount == %d\n",MoveCount);
  return MaterialScore;
}
