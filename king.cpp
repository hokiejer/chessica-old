#include "chess.h"

//
//PERFORMANCE PATH
//
int Reset::AddNextWhiteKingMove(Reset *MyChild, unsigned char To)
{
  MyChild->WhiteCastleQ = 0;
  MyChild->WhiteCastleK = 0;
  MyChild->WhiteKingSquare = To;
  MyChild->MustCheckSafety = ON;
  return(AddNextWhiteMove(MyChild,&(MyChild->bKings)));
}


//
//PERFORMANCE PATH
//
int Reset::AddNextBlackKingMove(Reset *MyChild, unsigned char To)
{
  MyChild->BlackCastleQ = 0;
  MyChild->BlackCastleK = 0;
  MyChild->BlackKingSquare = To;
  MyChild->MustCheckSafety = ON;
  return(AddNextBlackMove(MyChild,&(MyChild->bKings)));
}


//
//PERFORMANCE PATH
//
int Reset::GenerateNextWhiteKingMove(Reset *MyChild)
{
  switch(MoveNumber)
  {
    case 10:
      //Up and to the left
      UpdateMoveData(7);
      if ((ULEDGE & bCurrentPiece) && 
          (bMoveData & ~bWhite) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare+7)))
      {
        MoveNumber = 20;
        return(TRUE);
      }

    case 20:
      //Up
      UpdateMoveData(8);
      if ((CANMOVEUP & bCurrentPiece) && 
          (bMoveData & ~bWhite) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare+8)))
      {
        MoveNumber = 30;
        return(TRUE);
      }

    case 30:
      //Up and to the right
      UpdateMoveData(9);
      if ((UREDGE & bCurrentPiece) && 
          (bMoveData & ~bWhite) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare+9)))
      {
        MoveNumber = 40;
        return(TRUE);
      }

    case 40:
      //Right
      UpdateMoveData(1);
      if ((CANMOVERIGHT & bCurrentPiece) && 
          (bMoveData & ~bWhite) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare+1)))
      {
        MoveNumber = 50;
        return(TRUE);
      }

    case 50:
      //Down and to the right
      UpdateMoveData(-7);
      if ((DREDGE & bCurrentPiece) && 
          (bMoveData & ~bWhite) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare-7)))
      {
        MoveNumber = 60;
        return(TRUE);
      }

    case 60:
      //Down
      UpdateMoveData(-8);
      if ((CANMOVEDOWN & bCurrentPiece) && 
          (bMoveData & ~bWhite) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare-8)))
      {
        MoveNumber = 70;
        return(TRUE);
      }

    case 70:
      //Down and to the left
      UpdateMoveData(-9);
      if ((DLEDGE & bCurrentPiece) && 
          (bMoveData & ~bWhite) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare-9)))
      {
        MoveNumber = 80;
        return(TRUE);
      }

    case 80:
      //Left
      UpdateMoveData(-1);
      if ((CANMOVELEFT & bCurrentPiece) && 
          (bMoveData & ~bWhite) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare-1)))
      {
        MoveNumber = 90;
        return(TRUE);
      }

    case 90:
      //Castle kingside
      if (WhiteCastleK && 
          !(bAll & WCASTLEKEMPTY) && 
          WhiteIsSafe(WCASTLEKSAFE) &&
          (MyChild->MovesSinceCapture < 50))
      {
        MyChild->bFrom = 0x0800000000000000;
        MyChild->From = 4;
        MyChild->bTo = 0x0200000000000000;
        MyChild->To = 6;
        MyChild->bAll &= 0xf0ffffffffffffff; //remove the king & rook
        MyChild->bAll |= 0x0600000000000000; //place the king & rook
        MyChild->bWhite &= 0xf0ffffffffffffff; //remove the king & rook
        MyChild->bWhite |= 0x0600000000000000; //place the king & rook
        MyChild->bKings &= 0xf0ffffffffffffff; //remove the king
        MyChild->bKings |= 0x0200000000000000; //place the king
        MyChild->bRooks &= 0xf0ffffffffffffff; //remove the rook
        MyChild->bRooks |= 0x0400000000000000; //place the rook
        MyChild->HashValue = (int) (MyChild->bAll % LARGE_PRIME);
        MyChild->WhiteCastleQ = 0;
        MyChild->WhiteCastleK = 0;
        MyChild->KingCastled = 1;
        MyChild->WhiteKingSquare = 6;
        MyChild->MustCheckSafety = ON;
        if (MyChild->DidWhiteMoveCauseBlackCheck())
        {
          MyChild->BlackInCheck = ON;
        }
        MyChild->MovesSinceCapture = MovesSinceCapture + 1;
        MoveNumber = 100;
        return(TRUE);
      }

    case 100:
      //Castle queenside
      if (WhiteCastleQ && 
          !(bAll & WCASTLEQEMPTY) && 
          WhiteIsSafe(WCASTLEQSAFE) &&
          (MyChild->MovesSinceCapture < 50))
      {
        MyChild->bFrom = 0x0800000000000000;
        MyChild->From = 4;
        MyChild->bTo = 0x2000000000000000;
        MyChild->To = 2;
        MyChild->bAll &= 0x07ffffffffffffff; //remove the king & rook
        MyChild->bAll |= 0x3000000000000000; //place the king & rook
        MyChild->bWhite &= 0x07ffffffffffffff; //remove the king & rook
        MyChild->bWhite |= 0x3000000000000000; //place the king & rook
        MyChild->bKings &= 0x07ffffffffffffff; //remove the king
        MyChild->bKings |= 0x2000000000000000; //place the king
        MyChild->bRooks &= 0x07ffffffffffffff; //remove the rook
        MyChild->bRooks |= 0x1000000000000000; //place the rook
        MyChild->HashValue = (int) (MyChild->bAll % LARGE_PRIME);
        MyChild->WhiteCastleQ = 0;
        MyChild->WhiteCastleK = 0;
        MyChild->KingCastled = 1;
        MyChild->WhiteKingSquare = 2;
        MyChild->MustCheckSafety = ON;
        if (MyChild->DidWhiteMoveCauseBlackCheck())
        {
          MyChild->BlackInCheck = ON;
        }
        MyChild->MovesSinceCapture = MovesSinceCapture + 1;
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
int Reset::GenerateNextBlackKingMove(Reset *MyChild)
{
  switch(MoveNumber)
  {
    case 10:
      //Up and to the left
      UpdateMoveData(7);
      if ((ULEDGE & bCurrentPiece) && 
          (bMoveData & ~bBlack) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare+7)))
      {
        MoveNumber = 20;
        return(TRUE);
      }

    case 20:
      //Up
      UpdateMoveData(8);
      if ((CANMOVEUP & bCurrentPiece) && 
          (bMoveData & ~bBlack) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare+8)))
      {
        MoveNumber = 30;
        return(TRUE);
      }

    case 30:
      //Up and to the right
      UpdateMoveData(9);
      if ((UREDGE & bCurrentPiece) && 
          (bMoveData & ~bBlack) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare+9)))
      {
        MoveNumber = 40;
        return(TRUE);
      }

    case 40:
      //Right
      UpdateMoveData(1);
      if ((CANMOVERIGHT & bCurrentPiece) && 
          (bMoveData & ~bBlack) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare+1)))
      {
        MoveNumber = 50;
        return(TRUE);
      }

    case 50:
      //Down and to the right
      UpdateMoveData(-7);
      if ((DREDGE & bCurrentPiece) && 
          (bMoveData & ~bBlack) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare-7)))
      {
        MoveNumber = 60;
        return(TRUE);
      }

    case 60:
      //Down
      UpdateMoveData(-8);
      if ((CANMOVEDOWN & bCurrentPiece) && 
          (bMoveData & ~bBlack) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare-8)))
      {
        MoveNumber = 70;
        return(TRUE);
      }

    case 70:
      //Down and to the left
      UpdateMoveData(-9);
      if ((DLEDGE & bCurrentPiece) && 
          (bMoveData & ~bBlack) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare-9)))
      {
        MoveNumber = 80;
        return(TRUE);
      }

    case 80:
      //Left
      UpdateMoveData(-1);
      if ((CANMOVELEFT & bCurrentPiece) && 
          (bMoveData & ~bBlack) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare-1)))
      {
        MoveNumber = 90;
        return(TRUE);
      }

    case 90:
      //Castle kingside
      if (BlackCastleK && 
          !(bAll & BCASTLEKEMPTY) && 
          BlackIsSafe(BCASTLEKSAFE) &&
          (MyChild->MovesSinceCapture < 50))
      {
        MyChild->bFrom = 0x0000000000000008;
        MyChild->From = 60;
        MyChild->bTo = 0x0000000000000002;
        MyChild->To = 62;
        MyChild->bAll &= 0xfffffffffffffff0; //remove the king & rook
        MyChild->bAll |= 0x0000000000000006; //place the king & rook
        MyChild->bBlack &= 0xfffffffffffffff0; //remove the king & rook
        MyChild->bBlack |= 0x0000000000000006; //place the king & rook
        MyChild->bKings &= 0xfffffffffffffff0; //remove the king
        MyChild->bKings |= 0x0000000000000002; //place the king
        MyChild->bRooks &= 0xfffffffffffffff0; //remove the rook
        MyChild->bRooks |= 0x0000000000000004; //place the rook
        MyChild->HashValue = (int) (MyChild->bAll % LARGE_PRIME);
        MyChild->BlackCastleQ = 0;
        MyChild->BlackCastleK = 0;
        MyChild->KingCastled = 1;
        MyChild->BlackKingSquare = 62;
        MyChild->MustCheckSafety = ON;
        if (MyChild->DidBlackMoveCauseWhiteCheck())
        {
          MyChild->WhiteInCheck = ON;
        }
        MyChild->MovesSinceCapture = MovesSinceCapture + 1;
        MoveNumber = 100;
        return(TRUE);
      }

    case 100:
      //Castle queenside
      if (BlackCastleQ && 
          !(bAll & BCASTLEQEMPTY) && 
          BlackIsSafe(BCASTLEQSAFE) &&
          (MyChild->MovesSinceCapture < 50))
      {
        MyChild->bFrom = 0x0000000000000008;
        MyChild->From = 60;
        MyChild->bTo = 0x0000000000000020;
        MyChild->To = 58;
        MyChild->bAll &= 0xffffffffffffff07; //remove the king & rook
        MyChild->bAll |= 0x0000000000000030; //place the king & rook
        MyChild->bBlack &= 0xffffffffffffff07; //remove the king & rook
        MyChild->bBlack |= 0x0000000000000030; //place the king & rook
        MyChild->bKings &= 0xffffffffffffff07; //remove the king
        MyChild->bKings |= 0x0000000000000020; //place the king
        MyChild->bRooks &= 0xffffffffffffff07; //remove the rook
        MyChild->bRooks |= 0x0000000000000010; //place the rook
        MyChild->HashValue = (int) (MyChild->bAll % LARGE_PRIME);
        MyChild->BlackCastleQ = 0;
        MyChild->BlackCastleK = 0;
        MyChild->KingCastled = 1;
        MyChild->BlackKingSquare = 58;
        MyChild->MustCheckSafety = ON;
        if (MyChild->DidBlackMoveCauseWhiteCheck())
        {
          MyChild->WhiteInCheck = ON;
        }
        MyChild->MovesSinceCapture = MovesSinceCapture + 1;
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





int Reset::GenerateNextWhiteKingCapture(Reset *MyChild)
{
  switch(MoveNumber)
  {
    case 10:
      //Up and to the left
      UpdateMoveData(7);
      if ((ULEDGE & bCurrentPiece) && 
          (bMoveData & bBlack) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare+7)))
      {
        MoveNumber = 20;
        return(TRUE);
      }

    case 20:
      //Up
      UpdateMoveData(8);
      if ((CANMOVEUP & bCurrentPiece) && 
          (bMoveData & bBlack) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare+8)))
      {
        MoveNumber = 30;
        return(TRUE);
      }

    case 30:
      //Up and to the right
      UpdateMoveData(9);
      if ((UREDGE & bCurrentPiece) && 
          (bMoveData & bBlack) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare+9)))
      {
        MoveNumber = 40;
        return(TRUE);
      }

    case 40:
      //Right
      UpdateMoveData(1);
      if ((CANMOVERIGHT & bCurrentPiece) && 
          (bMoveData & bBlack) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare+1)))
      {
        MoveNumber = 50;
        return(TRUE);
      }

    case 50:
      //Down and to the right
      UpdateMoveData(-7);
      if ((DREDGE & bCurrentPiece) && 
          (bMoveData & bBlack) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare-7)))
      {
        MoveNumber = 60;
        return(TRUE);
      }

    case 60:
      //Down
      UpdateMoveData(-8);
      if ((CANMOVEDOWN & bCurrentPiece) && 
          (bMoveData & bBlack) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare-8)))
      {
        MoveNumber = 70;
        return(TRUE);
      }

    case 70:
      //Down and to the left
      UpdateMoveData(-9);
      if ((DLEDGE & bCurrentPiece) && 
          (bMoveData & bBlack) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare-9)))
      {
        MoveNumber = 80;
        return(TRUE);
      }

    case 80:
      //Left
      UpdateMoveData(-1);
      if ((CANMOVELEFT & bCurrentPiece) && 
          (bMoveData & bBlack) &&
          (AddNextWhiteKingMove(MyChild,WhiteKingSquare-1)))
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





int Reset::GenerateNextBlackKingCapture(Reset *MyChild)
{
  switch(MoveNumber)
  {
    case 10:
      //Up and to the left
      UpdateMoveData(7);
      if ((ULEDGE & bCurrentPiece) && 
          (bMoveData & bWhite) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare+7)))
      {
        MoveNumber = 20;
        return(TRUE);
      }

    case 20:
      //Up
      UpdateMoveData(8);
      if ((CANMOVEUP & bCurrentPiece) && 
          (bMoveData & bWhite) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare+8)))
      {
        MoveNumber = 30;
        return(TRUE);
      }

    case 30:
      //Up and to the right
      UpdateMoveData(9);
      if ((UREDGE & bCurrentPiece) && 
          (bMoveData & bWhite) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare+9)))
      {
        MoveNumber = 40;
        return(TRUE);
      }

    case 40:
      //Right
      UpdateMoveData(1);
      if ((CANMOVERIGHT & bCurrentPiece) && 
          (bMoveData & bWhite) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare+1)))
      {
        MoveNumber = 50;
        return(TRUE);
      }

    case 50:
      //Down and to the right
      UpdateMoveData(-7);
      if ((DREDGE & bCurrentPiece) && 
          (bMoveData & bWhite) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare-7)))
      {
        MoveNumber = 60;
        return(TRUE);
      }

    case 60:
      //Down
      UpdateMoveData(-8);
      if ((CANMOVEDOWN & bCurrentPiece) && 
          (bMoveData & bWhite) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare-8)))
      {
        MoveNumber = 70;
        return(TRUE);
      }

    case 70:
      //Down and to the left
      UpdateMoveData(-9);
      if ((DLEDGE & bCurrentPiece) && 
          (bMoveData & bWhite) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare-9)))
      {
        MoveNumber = 80;
        return(TRUE);
      }

    case 80:
      //Left
      UpdateMoveData(-1);
      if ((CANMOVELEFT & bCurrentPiece) && 
          (bMoveData & bWhite) &&
          (AddNextBlackKingMove(MyChild,BlackKingSquare-1)))
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



