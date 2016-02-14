#include "chess.h"

//
//PERFORMANCE PATH
//
int Reset::GenerateNextWhiteBishopMove(Reset *MyChild)
{
  switch(MoveNumber)
  {
    case 10:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 15;
    case 15:
      //Up and to the left
      while (ULEDGE & bMoveData)
      {
        bMoveData >>= 7;
        MoveData += 7;
        if ((bMoveData & ~bWhite) && (AddNextWhiteMove(MyChild,&(MyChild->bBishops))))
        {
          if (bMoveData & bBlack)
            MoveNumber = 20;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 20:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 25;
    case 25:
      //Up and to the right
      while (UREDGE & bMoveData)
      {
        bMoveData >>= 9;
        MoveData += 9;
        if ((bMoveData & ~bWhite) && (AddNextWhiteMove(MyChild,&(MyChild->bBishops))))
        {
          if (bMoveData & bBlack)
            MoveNumber = 30;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 30:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 35;
    case 35:
      //Down and to the left
      while (DLEDGE & bMoveData)
      {
        bMoveData <<= 9;
        MoveData -= 9;
        if ((bMoveData & ~bWhite) && (AddNextWhiteMove(MyChild,&(MyChild->bBishops))))
        {
          if (bMoveData & bBlack)
            MoveNumber = 40;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 40:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 45;
    case 45:
      //Down and to the right
      while (DREDGE & bMoveData)
      {
        bMoveData <<= 7;
        MoveData -= 7;
        if ((bMoveData & ~bWhite) && (AddNextWhiteMove(MyChild,&(MyChild->bBishops))))
        {
          if (bMoveData & bBlack)
          {
            MoveNumber = 10;
            bCurrentPiece >>= 1;
            CurrentPiece++;
          }
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
  }
  MoveNumber = 10;
  bCurrentPiece >>= 1;
  CurrentPiece++;
  return(FALSE);
}





//
//PERFORMANCE PATH
//
int Reset::GenerateNextBlackBishopMove(Reset *MyChild)
{
  switch(MoveNumber)
  {
    case 10:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 15;
    case 15:
      //Up and to the left
      while (ULEDGE & bMoveData)
      {
        bMoveData >>= 7;
        MoveData += 7;
        if ((bMoveData & ~bBlack) && (AddNextBlackMove(MyChild,&(MyChild->bBishops))))
        {
          if (bMoveData & bWhite)
            MoveNumber = 20;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 20:
      MoveNumber = 25;
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
    case 25:
      //Up and to the right
      while (UREDGE & bMoveData)
      {
        bMoveData >>= 9;
        MoveData += 9;
        if ((bMoveData & ~bBlack) && (AddNextBlackMove(MyChild,&(MyChild->bBishops))))
        {
          if (bMoveData & bWhite)
            MoveNumber = 30;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 30:
      MoveNumber = 35;
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
    case 35:
      //Down and to the left
      while (DLEDGE & bMoveData)
      {
        bMoveData <<= 9;
        MoveData -= 9;
        if ((bMoveData & ~bBlack) && (AddNextBlackMove(MyChild,&(MyChild->bBishops))))
        {
          if (bMoveData & bWhite)
            MoveNumber = 40;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 40:
      MoveNumber = 45;
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
    case 45:
      //Down and to the right
      while (DREDGE & bMoveData)
      {
        bMoveData <<= 7;
        MoveData -= 7;
        if ((bMoveData & ~bBlack) && (AddNextBlackMove(MyChild,&(MyChild->bBishops))))
        {
          if (bMoveData & bWhite)
          {
            MoveNumber = 10;
            bCurrentPiece >>= 1;
            CurrentPiece++;
          }
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
  }
  MoveNumber = 10;
  bCurrentPiece >>= 1;
  CurrentPiece++;
  return(FALSE);
}




int Reset::GenerateNextWhiteBishopCapture(Reset *MyChild)
{
  switch(MoveNumber)
  {
    case 10:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 15;
    case 15:
      //Up and to the left
      while (ULEDGE & bMoveData)
      {
        bMoveData >>= 7;
        MoveData += 7;
        if ((bMoveData & bBlack) && (AddNextWhiteMove(MyChild,&(MyChild->bBishops))))
        {
          MoveNumber = 20;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 20:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 25;
    case 25:
      //Up and to the right
      while (UREDGE & bMoveData)
      {
        bMoveData >>= 9;
        MoveData += 9;
        if ((bMoveData & bBlack) && (AddNextWhiteMove(MyChild,&(MyChild->bBishops))))
        {
          MoveNumber = 30;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 30:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 35;
    case 35:
      //Down and to the left
      while (DLEDGE & bMoveData)
      {
        bMoveData <<= 9;
        MoveData -= 9;
        if ((bMoveData & bBlack) && (AddNextWhiteMove(MyChild,&(MyChild->bBishops))))
        {
          MoveNumber = 40;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 40:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 45;
    case 45:
      //Down and to the right
      while (DREDGE & bMoveData)
      {
        bMoveData <<= 7;
        MoveData -= 7;
        if ((bMoveData & bBlack) && (AddNextWhiteMove(MyChild,&(MyChild->bBishops))))
        {
          MoveNumber = 10;
          bCurrentPiece >>= 1;
          CurrentPiece++;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
  }
  MoveNumber = 10;
  bCurrentPiece >>= 1;
  CurrentPiece++;
  return(FALSE);
}





int Reset::GenerateNextBlackBishopCapture(Reset *MyChild)
{
  switch(MoveNumber)
  {
    case 10:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 15;
    case 15:
      //Up and to the left
      while (ULEDGE & bMoveData)
      {
        bMoveData >>= 7;
        MoveData += 7;
        if ((bMoveData & bWhite) && (AddNextBlackMove(MyChild,&(MyChild->bBishops))))
        {
          MoveNumber = 20;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 20:
      MoveNumber = 25;
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
    case 25:
      //Up and to the right
      while (UREDGE & bMoveData)
      {
        bMoveData >>= 9;
        MoveData += 9;
        if ((bMoveData & bWhite) && (AddNextBlackMove(MyChild,&(MyChild->bBishops))))
        {
          MoveNumber = 30;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 30:
      MoveNumber = 35;
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
    case 35:
      //Down and to the left
      while (DLEDGE & bMoveData)
      {
        bMoveData <<= 9;
        MoveData -= 9;
        if ((bMoveData & bWhite) && (AddNextBlackMove(MyChild,&(MyChild->bBishops))))
        {
          MoveNumber = 40;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 40:
      MoveNumber = 45;
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
    case 45:
      //Down and to the right
      while (DREDGE & bMoveData)
      {
        bMoveData <<= 7;
        MoveData -= 7;
        if ((bMoveData & bWhite) && (AddNextBlackMove(MyChild,&(MyChild->bBishops))))
        {
          MoveNumber = 10;
          bCurrentPiece >>= 1;
          CurrentPiece++;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
  }
  MoveNumber = 10;
  bCurrentPiece >>= 1;
  CurrentPiece++;
  return(FALSE);
}




