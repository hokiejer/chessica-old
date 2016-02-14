#include "chess.h"


//
//PERFORMANCE PATH
//
int Reset::GenerateNextWhiteKnightMove(Reset *Child)
{
  switch(MoveNumber)
  {
    case 10:
      if ((bCurrentPiece & K1000) && 
          (UpdateMoveData(6) & ~bWhite) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 20;
        return(TRUE);
      }

    case 20:
      if ((bCurrentPiece & K1100) && 
          (UpdateMoveData(15) & ~bWhite) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 30;
        return(TRUE);
      }

    case 30:
      if ((bCurrentPiece & K0100) && 
          (UpdateMoveData(17) & ~bWhite) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 40;
        return(TRUE);
      }

    case 40:
      if ((bCurrentPiece & K0200) && 
          (UpdateMoveData(10) & ~bWhite) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 50;
        return(TRUE);
      }

    case 50:
      if ((bCurrentPiece & K0400) && 
          (UpdateMoveData(-6) & ~bWhite) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 60;
        return(TRUE);
      }

    case 60:
      if ((bCurrentPiece & K0500) && 
          (UpdateMoveData(-15) & ~bWhite) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 70;
        return(TRUE);
      }

    case 70:
      if ((bCurrentPiece & K0700) && 
          (UpdateMoveData(-17) & ~bWhite) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 80;
        return(TRUE);
      }

    case 80:
      if ((bCurrentPiece & K0800) && 
          (UpdateMoveData(-10) & ~bWhite) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 10;
        bCurrentPiece >>= 1;
        CurrentPiece++;
        return(TRUE);
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
int Reset::GenerateNextBlackKnightMove(Reset *Child)
{
  switch(MoveNumber)
  {
    case 10:
      if ((bCurrentPiece & K1000) && 
          (UpdateMoveData(6) & ~bBlack) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 20;
        return(TRUE);
      }

    case 20:
      if ((bCurrentPiece & K1100) && 
          (UpdateMoveData(15) & ~bBlack) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 30;
        return(TRUE);
      }

    case 30:
      if ((bCurrentPiece & K0100) && 
          (UpdateMoveData(17) & ~bBlack) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 40;
        return(TRUE);
      }

    case 40:
      if ((bCurrentPiece & K0200) && 
          (UpdateMoveData(10) & ~bBlack) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 50;
        return(TRUE);
      }

    case 50:
      if ((bCurrentPiece & K0400) && 
          (UpdateMoveData(-6) & ~bBlack) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 60;
        return(TRUE);
      }

    case 60:
      if ((bCurrentPiece & K0500) && 
          (UpdateMoveData(-15) & ~bBlack) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 70;
        return(TRUE);
      }

    case 70:
      if ((bCurrentPiece & K0700) && 
          (UpdateMoveData(-17) & ~bBlack) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 80;
        return(TRUE);
      }

    case 80:
      if ((bCurrentPiece & K0800) && 
          (UpdateMoveData(-10) & ~bBlack) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 10;
        bCurrentPiece >>= 1;
        CurrentPiece++;
        return(TRUE);
      }

    default:
      MoveNumber = 10;
      bCurrentPiece >>= 1;
      CurrentPiece++;
      return(FALSE);
  }
}




int Reset::GenerateNextWhiteKnightCapture(Reset *Child)
{
  switch(MoveNumber)
  {
    case 10:
      if ((bCurrentPiece & K1000) && 
          (UpdateMoveData(6) & bBlack) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 20;
        return(TRUE);
      }

    case 20:
      if ((bCurrentPiece & K1100) && 
          (UpdateMoveData(15) & bBlack) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 30;
        return(TRUE);
      }

    case 30:
      if ((bCurrentPiece & K0100) && 
          (UpdateMoveData(17) & bBlack) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 40;
        return(TRUE);
      }

    case 40:
      if ((bCurrentPiece & K0200) && 
          (UpdateMoveData(10) & bBlack) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 50;
        return(TRUE);
      }

    case 50:
      if ((bCurrentPiece & K0400) && 
          (UpdateMoveData(-6) & bBlack) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 60;
        return(TRUE);
      }

    case 60:
      if ((bCurrentPiece & K0500) && 
          (UpdateMoveData(-15) & bBlack) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 70;
        return(TRUE);
      }

    case 70:
      if ((bCurrentPiece & K0700) && 
          (UpdateMoveData(-17) & bBlack) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 80;
        return(TRUE);
      }

    case 80:
      if ((bCurrentPiece & K0800) && 
          (UpdateMoveData(-10) & bBlack) &&
          (AddNextWhiteMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 10;
        bCurrentPiece >>= 1;
        CurrentPiece++;
        return(TRUE);
      }

    default:
      MoveNumber = 10;
      bCurrentPiece >>= 1;
      CurrentPiece++;
      return(FALSE);
  }
}





int Reset::GenerateNextBlackKnightCapture(Reset *Child)
{
  switch(MoveNumber)
  {
    case 10:
      if ((bCurrentPiece & K1000) && 
          (UpdateMoveData(6) & bWhite) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 20;
        return(TRUE);
      }

    case 20:
      if ((bCurrentPiece & K1100) && 
          (UpdateMoveData(15) & bWhite) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 30;
        return(TRUE);
      }

    case 30:
      if ((bCurrentPiece & K0100) && 
          (UpdateMoveData(17) & bWhite) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 40;
        return(TRUE);
      }

    case 40:
      if ((bCurrentPiece & K0200) && 
          (UpdateMoveData(10) & bWhite) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 50;
        return(TRUE);
      }

    case 50:
      if ((bCurrentPiece & K0400) && 
          (UpdateMoveData(-6) & bWhite) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 60;
        return(TRUE);
      }

    case 60:
      if ((bCurrentPiece & K0500) && 
          (UpdateMoveData(-15) & bWhite) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 70;
        return(TRUE);
      }

    case 70:
      if ((bCurrentPiece & K0700) && 
          (UpdateMoveData(-17) & bWhite) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 80;
        return(TRUE);
      }

    case 80:
      if ((bCurrentPiece & K0800) && 
          (UpdateMoveData(-10) & bWhite) &&
          (AddNextBlackMove(Child,&(Child->bKnights))))
      {
        MoveNumber = 10;
        bCurrentPiece >>= 1;
        CurrentPiece++;
        return(TRUE);
      }

    default:
      MoveNumber = 10;
      bCurrentPiece >>= 1;
      CurrentPiece++;
      return(FALSE);
  }
}




