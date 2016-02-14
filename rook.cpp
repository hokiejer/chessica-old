#include "chess.h"

//
//PERFORMANCE PATH
//
int Reset::AddNextWhiteRookMove(Reset *MyChild)
{
  if (bCurrentPiece & LLCORNER)
    MyChild->WhiteCastleQ = 0;
  if (bCurrentPiece & LRCORNER)
    MyChild->WhiteCastleK = 0;
  return(AddNextWhiteMove(MyChild,&(MyChild->bRooks)));
}


//
//PERFORMANCE PATH
//
int Reset::AddNextBlackRookMove(Reset *MyChild)
{
  if (bCurrentPiece & ULCORNER)
    MyChild->BlackCastleQ = 0;
  if (bCurrentPiece & URCORNER)
    MyChild->BlackCastleK = 0;
  return (AddNextBlackMove(MyChild,&(MyChild->bRooks)));
}


//
//PERFORMANCE PATH
//
int Reset::GenerateNextWhiteRookMove(Reset *MyChild)
{
  switch(MoveNumber)
  {
    case 10:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 15;
    case 15:
      //Up
      while (CANMOVEUP & bMoveData)
      {
        bMoveData >>= 8;
        MoveData += 8;
        if ((bMoveData & ~bWhite) && (AddNextWhiteRookMove(MyChild)))
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
      //Down
      while (CANMOVEDOWN & bMoveData)
      {
        bMoveData <<= 8;
        MoveData -= 8;
        if ((bMoveData & ~bWhite) && (AddNextWhiteRookMove(MyChild)))
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
      //Left
      while (CANMOVELEFT & bMoveData)
      {
        bMoveData <<= 1;
        MoveData -= 1;
        if ((bMoveData & ~bWhite) && (AddNextWhiteRookMove(MyChild)))
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
      //Right
      while (CANMOVERIGHT & bMoveData)
      {
        bMoveData >>= 1;
        MoveData += 1;
        if ((bMoveData & ~bWhite) && (AddNextWhiteRookMove(MyChild)))
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

    default:
      MoveNumber = 10;
      bCurrentPiece >>= 1;
      CurrentPiece++;
      return(FALSE);
  }
}





//
//PERFORMANCE PATH
//
int Reset::GenerateNextBlackRookMove(Reset *MyChild)
{
  switch(MoveNumber)
  {
    case 10:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 15;
    case 15:
      //Up
      while (CANMOVEUP & bMoveData)
      {
        bMoveData >>= 8;
        MoveData += 8;
        if ((bMoveData & ~bBlack) && (AddNextBlackRookMove(MyChild)))
        {
          if (bMoveData & bWhite)
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
      //Down
      while (CANMOVEDOWN & bMoveData)
      {
        bMoveData <<= 8;
        MoveData -= 8;
        if ((bMoveData & ~bBlack) && (AddNextBlackRookMove(MyChild)))
        {
          if (bMoveData & bWhite)
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
      //Left
      while (CANMOVELEFT & bMoveData)
      {
        bMoveData <<= 1;
        MoveData -= 1;
        if ((bMoveData & ~bBlack) && (AddNextBlackRookMove(MyChild)))
        {
          if (bMoveData & bWhite)
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
      //Right
      while (CANMOVERIGHT & bMoveData)
      {
        bMoveData >>= 1;
        MoveData += 1;
        if ((bMoveData & ~bBlack) && (AddNextBlackRookMove(MyChild)))
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

    default:
      MoveNumber = 10;
      bCurrentPiece >>= 1;
      CurrentPiece++;
      return(FALSE);
  }
}






int Reset::GenerateNextWhiteRookCapture(Reset *MyChild)
{
  switch(MoveNumber)
  {
    case 10:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 15;
    case 15:
      //Up
      while (CANMOVEUP & bMoveData)
      {
        bMoveData >>= 8;
        MoveData += 8;
        if ((bMoveData & bBlack) && (AddNextWhiteRookMove(MyChild)))
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
      //Down
      while (CANMOVEDOWN & bMoveData)
      {
        bMoveData <<= 8;
        MoveData -= 8;
        if ((bMoveData & bBlack) && (AddNextWhiteRookMove(MyChild)))
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
      //Left
      while (CANMOVELEFT & bMoveData)
      {
        bMoveData <<= 1;
        MoveData -= 1;
        if ((bMoveData & bBlack) && (AddNextWhiteRookMove(MyChild)))
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
      //Right
      while (CANMOVERIGHT & bMoveData)
      {
        bMoveData >>= 1;
        MoveData += 1;
        if ((bMoveData & bBlack) && (AddNextWhiteRookMove(MyChild)))
        {
          MoveNumber = 10;
          bCurrentPiece >>= 1;
          CurrentPiece++;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    default:
      MoveNumber = 10;
      bCurrentPiece >>= 1;
      CurrentPiece++;
      return(FALSE);
  }
}





int Reset::GenerateNextBlackRookCapture(Reset *MyChild)
{
  switch(MoveNumber)
  {
    case 10:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 15;
    case 15:
      //Up
      while (CANMOVEUP & bMoveData)
      {
        bMoveData >>= 8;
        MoveData += 8;
        if ((bMoveData & bWhite) && (AddNextBlackRookMove(MyChild)))
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
      //Down
      while (CANMOVEDOWN & bMoveData)
      {
        bMoveData <<= 8;
        MoveData -= 8;
        if ((bMoveData & bWhite) && (AddNextBlackRookMove(MyChild)))
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
      //Left
      while (CANMOVELEFT & bMoveData)
      {
        bMoveData <<= 1;
        MoveData -= 1;
        if ((bMoveData & bWhite) && (AddNextBlackRookMove(MyChild)))
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
      //Right
      while (CANMOVERIGHT & bMoveData)
      {
        bMoveData >>= 1;
        MoveData += 1;
        if ((bMoveData & bWhite) && (AddNextBlackRookMove(MyChild)))
        {
          MoveNumber = 10;
          bCurrentPiece >>= 1;
          CurrentPiece++;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    default:
      MoveNumber = 10;
      bCurrentPiece >>= 1;
      CurrentPiece++;
      return(FALSE);
  }
}
