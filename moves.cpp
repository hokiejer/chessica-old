#include "chess.h"



//
//PERFORMANCE PATH
//
int Reset::AddNextWhiteMove(Reset *MyChild, unsigned long long int *PieceBeingMoved)
{
  MyChild->bFrom = bCurrentPiece;
  MyChild->From = CurrentPiece;
  MyChild->bAll &= ~MyChild->bFrom;
  MyChild->bWhite &= ~MyChild->bFrom;
  *PieceBeingMoved &= ~MyChild->bFrom;
  MyChild->bTo = bMoveData;
  MyChild->To = MoveData;
  if (MyChild->bTo & MyChild->bAll)
  {
    MyChild->CaptureProcessing(MyChild->bTo);
    MyChild->Capture = 1;
    MyChild->MovesSinceCapture = 0; //Reset on capture
  }
  else
  {
    if (MyChild->bTo & MyChild->bPawns)
      MyChild->MovesSinceCapture = 0; //Reset on pawn move
    else
      MyChild->MovesSinceCapture = MovesSinceCapture + 1;
  }
  MyChild->bAll |= MyChild->bTo;
  MyChild->bWhite |= MyChild->bTo;
  *PieceBeingMoved |= MyChild->bTo;
  if ((MyChild->DidWhiteJustMoveIntoCheck()) || 
      (MyChild->MovesSinceCapture >= 50))
  {
    InitMyChild(MyChild);
    return(FALSE);
  }
  if (MyChild->DidWhiteMoveCauseBlackCheck())
  {
    MyChild->BlackInCheck = ON;
  }
  MyChild->HashValue = (int) (MyChild->bAll % LARGE_PRIME);
  return(TRUE);
}


//
//PERFORMANCE PATH
//
int Reset::AddNextBlackMove(Reset *MyChild, unsigned long long int *PieceBeingMoved)
{
  MyChild->bFrom = bCurrentPiece;
  MyChild->From = CurrentPiece;
  MyChild->bAll &= ~MyChild->bFrom;
  MyChild->bBlack &= ~MyChild->bFrom;
  *PieceBeingMoved &= ~MyChild->bFrom;
  MyChild->bTo = bMoveData;
  MyChild->To = MoveData;
  if (MyChild->bTo & MyChild->bAll)
  {
    MyChild->CaptureProcessing(MyChild->bTo);
    MyChild->Capture = 1;
    MyChild->MovesSinceCapture = 0; //Reset on capture
  }
  else
  {
    if (MyChild->bTo & MyChild->bPawns)
      MyChild->MovesSinceCapture = 0; //Reset on pawn move
    else
      MyChild->MovesSinceCapture = MovesSinceCapture + 1;
  }
  MyChild->bAll |= MyChild->bTo;
  MyChild->bBlack |= MyChild->bTo;
  *PieceBeingMoved |= MyChild->bTo;
  if ((MyChild->DidBlackJustMoveIntoCheck()) || 
      (MyChild->MovesSinceCapture >= 50))
  {
    InitMyChild(MyChild);
    return(FALSE);
  }
  if (MyChild->DidBlackMoveCauseWhiteCheck())
  {
    MyChild->WhiteInCheck = ON;
  }
  MyChild->HashValue = (int) (MyChild->bAll % LARGE_PRIME);
  return(TRUE);
}


unsigned long long int Reset::UpdateMoveData(int Target)
{
  MoveData = CurrentPiece + Target;
  if (Target > 0)
  {
    return (bMoveData = bCurrentPiece >> Target);
  }
  else
  {
    return (bMoveData = bCurrentPiece << -Target);
  }
}



//
//PERFORMANCE PATH
//
void Reset::InitializeMoveGeneration()
{
  bCurrentPiece = 0x8000000000000000;
  CurrentPiece = 0;
  MoveNumber = 10;	//Prime the first move
}



int Reset::NoMovesGenerated()
{
  return ((CurrentPiece == 0) && (MoveNumber == 10));
}



//
//PERFORMANCE PATH
//
//Note: This gets called once per piecemove, not once per piece.  Queen moves should be second
int Reset::GenerateNextMove(Reset *Target)
{
  int retcode = FALSE;

  if (ToMove) 		//If it is WHITE's move
  {
    while (bCurrentPiece)
    {
      if (bCurrentPiece & bWhite)
      {
        if (bCurrentPiece & bPawns)
        {
          if (retcode = GenerateNextWhitePawnMove(Target))
            break;
        }
        else
        {
          if (bCurrentPiece & bKnights)
          {
            if (retcode = GenerateNextWhiteKnightMove(Target))
              break;
          }
          else
          {
            if (bCurrentPiece & bBishops)
            {
              if (retcode = GenerateNextWhiteBishopMove(Target))
                break;
            }
            else
            {
              if (bCurrentPiece & bRooks)
              {
                if (retcode = GenerateNextWhiteRookMove(Target))
                  break;
              }
              else
              {
                if (bCurrentPiece & bQueens)
                {
                  if (retcode = GenerateNextWhiteQueenMove(Target))
                    break;
                }
                else //King
                {
                  if (retcode = GenerateNextWhiteKingMove(Target))
                    break;
                }
              }
            }
          }
        }
      }
      else
      {
        //Piece routines will advance bCurrentPiece by themselves
        bCurrentPiece >>= 1;
        CurrentPiece++;
      }
    }
  }
  else // Black's Move
  {
    while (bCurrentPiece)
    {
      if (bCurrentPiece & bBlack)
      {
        if (bCurrentPiece & bPawns)
        {
          if (retcode = GenerateNextBlackPawnMove(Target))
            break;
        }
        else
        {
          if (bCurrentPiece & bKnights)
          {
            if (retcode = GenerateNextBlackKnightMove(Target))
              break;
          }
          else
          {
            if (bCurrentPiece & bBishops)
            {
              if (retcode = GenerateNextBlackBishopMove(Target))
                break;
            }
            else
            {
              if (bCurrentPiece & bRooks)
              {
                if (retcode = GenerateNextBlackRookMove(Target))
                  break;
              }
              else
              {
                if (bCurrentPiece & bQueens)
                {
                  if (retcode = GenerateNextBlackQueenMove(Target))
                    break;
                }
                else //King
                {
                  if (retcode = GenerateNextBlackKingMove(Target))
                    break;
                }
              }
            }
          }
        }
      }
      else
      {
        //Piece routines will advance bCurrentPiece by themselves
        bCurrentPiece >>= 1;
        CurrentPiece++;
      }
    }
  }
  return(retcode);
}




int Reset::GenerateNextCapture(Reset *Target)
{
  int retcode = FALSE;

  if (ToMove) 		//If it is WHITE's move
  {
    while (bCurrentPiece)
    {
      if (bCurrentPiece & bWhite)
      {
        if (bCurrentPiece & bPawns)
        {
          if (retcode = GenerateNextWhitePawnCapture(Target))
            break;
        }
        else
        {
          if (bCurrentPiece & bKnights)
          {
            if (retcode = GenerateNextWhiteKnightCapture(Target))
              break;
          }
          else
          {
            if (bCurrentPiece & bBishops)
            {
              if (retcode = GenerateNextWhiteBishopCapture(Target))
                break;
            }
            else
            {
              if (bCurrentPiece & bRooks)
              {
                if (retcode = GenerateNextWhiteRookCapture(Target))
                  break;
              }
              else
              {
                if (bCurrentPiece & bQueens)
                {
                  if (retcode = GenerateNextWhiteQueenCapture(Target))
                    break;
                }
                else //King
                {
                  if (retcode = GenerateNextWhiteKingCapture(Target))
                    break;
                }
              }
            }
          }
        }
      }
      else
      {
        //Piece routines will advance bCurrentPiece by themselves
        bCurrentPiece >>= 1;
        CurrentPiece++;
      }
    }
  }
  else // Black's Move
  {
    while (bCurrentPiece)
    {
      if (bCurrentPiece & bBlack)
      {
        if (bCurrentPiece & bPawns)
        {
          if (retcode = GenerateNextBlackPawnCapture(Target))
            break;
        }
        else
        {
          if (bCurrentPiece & bKnights)
          {
            if (retcode = GenerateNextBlackKnightCapture(Target))
              break;
          }
          else
          {
            if (bCurrentPiece & bBishops)
            {
              if (retcode = GenerateNextBlackBishopCapture(Target))
                break;
            }
            else
            {
              if (bCurrentPiece & bRooks)
              {
                if (retcode = GenerateNextBlackRookCapture(Target))
                  break;
              }
              else
              {
                if (bCurrentPiece & bQueens)
                {
                  if (retcode = GenerateNextBlackQueenCapture(Target))
                    break;
                }
                else //King
                {
                  if (retcode = GenerateNextBlackKingCapture(Target))
                    break;
                }
              }
            }
          }
        }
      }
      else
      {
        //Piece routines will advance bCurrentPiece by themselves
        bCurrentPiece >>= 1;
        CurrentPiece++;
      }
    }
  }
  return(retcode);
}

