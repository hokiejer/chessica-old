#include "chess.h"

Reset Reset::FreeResetList;


Reset::Reset()		//constructor
{
  
}


//
//PERFORMANCE PATH
//
void Reset::InitMyChild(Reset * Child)
{
  int i;

  //memcpy(Child->Board,Board,137);

  memcpy((void *) &(Child->bAll),(void *) &(bAll),80);
  //Child->bAll = bAll;			//8 bytes
  //Child->bWhite = bWhite;		//8 bytes
  //Child->bBlack = bBlack;		//8 bytes
  //Child->bPawns = bPawns;		//8 bytes
  //Child->bKnights = bKnights;		//8 bytes
  //Child->bBishops = bBishops;		//8 bytes
  //Child->bRooks = bRooks;		//8 bytes
  //Child->bQueens = bQueens;		//8 bytes
  //Child->bKings = bKings;		//8 bytes
  //Child->Material = Material;		//1 byte
  //Child->MovesSinceCapture = MovesSinceCapture;	//1 byte
  //Child->WhiteKingSquare = WhiteKingSquare;	//1 byte
  //Child->BlackKingSquare = BlackKingSquare;	//1 byte
  //Childd bits				//1 byte

  memset((void *) &(Child->bCurrentPiece), 0, 32);
  //Child->bCurrentPiece = 0;
  //Child->bEP = 0;
  //Child->bMoveData = 0;
  //Child->Score = 0;
  //Child->MoveNumber = 0;
  //Child->CurrentPiece = 0;
  //Child->MoveData = 0;

  //Bits
  //Child->Capture = OFF;
  //Child->WhiteInCheck = OFF;
  //Child->BlackInCheck = OFF;
  //Child->ToMove = BLACK; //Same as OFF
  //Child->EPCapture = OFF;
  //Child->Promotion = OFF;
  //Child->KingCastled = OFF;
  //Child->GameOver = OFF;

  if (ToMove == BLACK)
    Child->ToMove = WHITE;
  Child->MustCheckSafety = (WhiteInCheck || BlackInCheck);
}


int Reset::GameNotOver()
{
  if (GameOver)
    return FALSE;
  else
    return TRUE;
}


//
//PERFORMANCE PATH
//
int Reset::FullHashesMatch(Reset *Candidate)
{
  if (bAll == Candidate->bAll)
    return TRUE;
  else
    return FALSE;
}



//
//PERFORMANCE PATH
//
int Reset::ChildrenMatch(Reset *Candidate)
{
  //
  //This is an abbreviated version of "ResetMatches" that will only work on
  //children of the same parent
  //
  if (bAll != Candidate->bAll)
    return FALSE;
  if (Promotion)
  {
    if (bKnights != Candidate->bKnights)
      return FALSE;
    if (bBishops != Candidate->bBishops)
      return FALSE;
    if (bRooks != Candidate->bRooks)
      return FALSE;
    if (bQueens != Candidate->bQueens)
      return FALSE;
  }
  return TRUE;
}



int Reset::ResetMatches(Reset *Candidate)
{
  if (bAll != Candidate->bAll)
    return FALSE;
  if (bWhite != Candidate->bWhite)
    return FALSE;
  if (bPawns != Candidate->bPawns)
    return FALSE;
  if (bKnights != Candidate->bKnights)
    return FALSE;
  if (bBishops != Candidate->bBishops)
    return FALSE;
  if (bRooks != Candidate->bRooks)
    return FALSE;
  if (bQueens != Candidate->bQueens)
    return FALSE;
  if (WhiteCastleQ != Candidate->WhiteCastleQ)
    return FALSE;
  if (WhiteCastleK != Candidate->WhiteCastleK)
    return FALSE;
  if (BlackCastleQ != Candidate->BlackCastleQ)
    return FALSE;
  if (BlackCastleK != Candidate->BlackCastleK)
    return FALSE;

  return TRUE;
}


//
//PERFORMANCE PATH
//
void Reset::CopyReset(Reset *Target)
{
  memcpy(&(Target->bAll),&(bAll),80+32+36);
}


//
//PERFORMANCE PATH
//
int Reset::WhiteToMove()
{
  return (ToMove == WHITE);
}

