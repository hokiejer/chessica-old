#include "chess.h"

/* To is the location of the captured piece */
void Reset::CaptureProcessing(unsigned long long int To)
{
  //Note that we're looking at the child, so ToMove is the NEXT to move
  //Remove Captured Piece and adjust Material
  //Always perform the capture BEFORE the attacking piece moves
  bWhite &= ~To;  //Doing both is faster than deciding
  bBlack &= ~To;
  bAll &= ~To;
  if (bPawns & To)
  {
    Material -= Points[ToMove][PAWN];
    bPawns &= ~To;
  }
  else 
  {
    if (bKnights & To)
    {
      Material -= Points[ToMove][KNIGHT];
      bKnights &= ~To;
    }
    else
    {
      if (bBishops & To)
      {
        Material -= Points[ToMove][BISHOP];
        bBishops &= ~To;
      }
      else
      {
        if (bRooks & To)
        {
          Material -= Points[ToMove][ROOK];
          bRooks &= ~To;
          if (To & ULCORNER)
            BlackCastleQ = 0;
          if (To & URCORNER)
            BlackCastleK = 0;
          if (To & LLCORNER)
            WhiteCastleQ = 0;
          if (To & LRCORNER)
            WhiteCastleK = 0;
        }
        else //Queen
        {
          Material -= Points[ToMove][QUEEN];
          bQueens &= ~To;
        }
      }
    }
  }  
}

