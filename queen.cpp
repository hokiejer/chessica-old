#include "chess.h"


//
//PERFORMANCE PATH
//
int Reset::GenerateNextWhiteQueenMove(Reset *MyChild)
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
        if ((bMoveData & ~bWhite) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
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
        if ((bMoveData & ~bWhite) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
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
        if ((bMoveData & ~bWhite) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
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
        if ((bMoveData & ~bWhite) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
        {
          if (bMoveData & bBlack)
            MoveNumber = 50;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 50:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 55;
    case 55:
      //Up
      while (CANMOVEUP & bMoveData)
      {
        bMoveData >>= 8;
        MoveData += 8;
        if ((bMoveData & ~bWhite) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
        {
          if (bMoveData & bBlack)
            MoveNumber = 60;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 60:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 65;
    case 65:
      //Down
      while (CANMOVEDOWN & bMoveData)
      {
        bMoveData <<= 8;
        MoveData -= 8;
        if ((bMoveData & ~bWhite) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
        {
          if (bMoveData & bBlack)
            MoveNumber = 70;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
    
    case 70:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 75;
    case 75:
      //Left
      while (CANMOVELEFT & bMoveData)
      {
        bMoveData <<= 1;
        MoveData -= 1;
        if ((bMoveData & ~bWhite) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
        {
          if (bMoveData & bBlack)
            MoveNumber = 80;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
    
    case 80:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 85;
    case 85:
      //Right
      while (CANMOVERIGHT & bMoveData)
      {
        bMoveData >>= 1;
        MoveData += 1;
        if ((bMoveData & ~bWhite) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
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
int Reset::GenerateNextBlackQueenMove(Reset *MyChild)
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
        if ((bMoveData & ~bBlack) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
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
      //Up and to the right
      while (UREDGE & bMoveData)
      {
        bMoveData >>= 9;
        MoveData += 9;
        if ((bMoveData & ~bBlack) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
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
      //Down and to the left
      while (DLEDGE & bMoveData)
      {
        bMoveData <<= 9;
        MoveData -= 9;
        if ((bMoveData & ~bBlack) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
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
      //Down and to the right
      while (DREDGE & bMoveData)
      {
        bMoveData <<= 7;
        MoveData -= 7;
        if ((bMoveData & ~bBlack) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
        {
          if (bMoveData & bWhite)
            MoveNumber = 50;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
    
    case 50:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 55;
    case 55:
      //Up
      while (CANMOVEUP & bMoveData)
      {
        bMoveData >>= 8;
        MoveData += 8;
        if ((bMoveData & ~bBlack) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
        {
          if (bMoveData & bWhite)
            MoveNumber = 60;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
    
    case 60:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 65;
    case 65:
      //Down
      while (CANMOVEDOWN & bMoveData)
      {
        bMoveData <<= 8;
        MoveData -= 8;
        if ((bMoveData & ~bBlack) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
        {
          if (bMoveData & bWhite)
            MoveNumber = 70;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
    
    case 70:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 75;
    case 75:
      //Left
      while (CANMOVELEFT & bMoveData)
      {
        bMoveData <<= 1;
        MoveData -= 1;
        if ((bMoveData & ~bBlack) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
        {
          if (bMoveData & bWhite)
            MoveNumber = 80;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
    
    case 80:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 85;
    case 85:
      //Right
      while (CANMOVERIGHT & bMoveData)
      {
        bMoveData >>= 1;
        MoveData += 1;
        if ((bMoveData & ~bBlack) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
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






int Reset::GenerateNextWhiteQueenCapture(Reset *MyChild)
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
        if ((bMoveData & bBlack) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
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
        if ((bMoveData & bBlack) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
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
        if ((bMoveData & bBlack) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
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
        if ((bMoveData & bBlack) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
        {
          MoveNumber = 50;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 50:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 55;
    case 55:
      //Up
      while (CANMOVEUP & bMoveData)
      {
        bMoveData >>= 8;
        MoveData += 8;
        if ((bMoveData & bBlack) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
        {
          MoveNumber = 60;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }

    case 60:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 65;
    case 65:
      //Down
      while (CANMOVEDOWN & bMoveData)
      {
        bMoveData <<= 8;
        MoveData -= 8;
        if ((bMoveData & bBlack) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
        {
          MoveNumber = 70;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
    
    case 70:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 75;
    case 75:
      //Left
      while (CANMOVELEFT & bMoveData)
      {
        bMoveData <<= 1;
        MoveData -= 1;
        if ((bMoveData & bBlack) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
        {
          MoveNumber = 80;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
    
    case 80:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 85;
    case 85:
      //Right
      while (CANMOVERIGHT & bMoveData)
      {
        bMoveData >>= 1;
        MoveData += 1;
        if ((bMoveData & bBlack) && (AddNextWhiteMove(MyChild,&(MyChild->bQueens))))
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





int Reset::GenerateNextBlackQueenCapture(Reset *MyChild)
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
        if ((bMoveData & bWhite) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
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
        if ((bMoveData & bWhite) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
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
        if ((bMoveData & bWhite) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
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
        if ((bMoveData & bWhite) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
        {
          MoveNumber = 50;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
    
    case 50:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 55;
    case 55:
      //Up
      while (CANMOVEUP & bMoveData)
      {
        bMoveData >>= 8;
        MoveData += 8;
        if ((bMoveData & bWhite) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
        {
          MoveNumber = 60;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
    
    case 60:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 65;
    case 65:
      //Down
      while (CANMOVEDOWN & bMoveData)
      {
        bMoveData <<= 8;
        MoveData -= 8;
        if ((bMoveData & bWhite) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
        {
          MoveNumber = 70;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
    
    case 70:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 75;
    case 75:
      //Left
      while (CANMOVELEFT & bMoveData)
      {
        bMoveData <<= 1;
        MoveData -= 1;
        if ((bMoveData & bWhite) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
        {
          MoveNumber = 80;
          return(TRUE);
        }
        if (bMoveData & bAll)
          break;
      }
    
    case 80:
      bMoveData = bCurrentPiece;
      MoveData = CurrentPiece;
      MoveNumber = 85;
    case 85:
      //Right
      while (CANMOVERIGHT & bMoveData)
      {
        bMoveData >>= 1;
        MoveData += 1;
        if ((bMoveData & bWhite) && (AddNextBlackMove(MyChild,&(MyChild->bQueens))))
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


