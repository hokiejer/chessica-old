#include "chess.h"


//
//PERFORMANCE PATH
//
int Reset::GenerateNextWhitePawnMove(Reset *Child)
{
  switch (MoveNumber / 10)
  {
    case 1:
      //One space forward: MoveNumber ==> 10,11,12,13,14
      if (UpdateMoveData(8) & ~bAll) 
      {
        //if not a promotion
        if (bCurrentPiece & NOTRANK7)
        {
          if (AddNextWhiteMove(Child,(&Child->bPawns)))
          {
            MoveNumber = 20; //Bookmark the next move to look at
            return(TRUE);
          }
        }
        else
        {
          MoveNumber++;
          if (GenerateNextWhitePawnPromotion(Child,bCurrentPiece >> 8))
            return(TRUE);
        }
      }

    case 2:
      //Two spaces forward: MoveNumber ==> 20
      if ((bCurrentPiece & RANK2) && 
          ((bCurrentPiece >> 8) & ~bAll) &&
          (UpdateMoveData(16) & ~bAll))
      {
        Child->bEP = bMoveData;
        if (AddNextWhiteMove(Child,(&Child->bPawns)))
        {
          MoveNumber = 30; //Bookmark the next move to look at
          return(TRUE);
        }
      }
      MoveNumber = 30;

    case 3:
      //Capture To My Left (Normal): MoveNumber ==> 30,31,32,33,34
      if ((bCurrentPiece & NOTFILE1) && (UpdateMoveData(7) & bBlack))
      {
        //if not a promotion
        if (bCurrentPiece & NOTRANK7)
        {
          if (AddNextWhiteMove(Child,(&Child->bPawns)))
          {
            MoveNumber = 40; //Bookmark the next move to look at
            return(TRUE);
          }
        }
        else
        {
          MoveNumber++;
          if (GenerateNextWhitePawnPromotion(Child,bCurrentPiece >> 7))
            return(TRUE);
        }
      }

    case 4:
      //Capture To My Left (en passant): MoveNumber ==> 40
      if (bCurrentPiece & NOTFILE1)
      {
        if ((bCurrentPiece << 1) & bEP)
        {
          Child->bFrom = bCurrentPiece;
          Child->From = CurrentPiece;
          Child->bAll &= ~Child->bFrom;
          Child->bWhite &= ~Child->bFrom;
          Child->bPawns &= ~Child->bFrom;

          Child->bTo = bCurrentPiece >> 7;
          Child->To = CurrentPiece + 7;
          Child->CaptureProcessing(bCurrentPiece << 1);
          Child->bAll |= Child->bTo;
          Child->bWhite |= Child->bTo;
          Child->bPawns |= Child->bTo;
          Child->Capture = 1;
          Child->EPCapture = 1;
          Child->MustCheckSafety = 1;
          if (Child->DidWhiteJustMoveIntoCheck())
          {
            InitMyChild(Child);
          }
          else
          {
            if (Child->DidWhiteMoveCauseBlackCheck())
            {
              Child->BlackInCheck = ON;
            }
            Child->MovesSinceCapture = 0;
            Child->HashValue = (int) (Child->bAll % LARGE_PRIME);
            MoveNumber = 50; //Bookmark the next move to look at
            return(TRUE);
          }
        }
      }
      MoveNumber = 50;

    case 5:
      //Capture To My Right (Normal): MoveNumber ==> 50,51,52,53,54
      if ((bCurrentPiece & NOTFILE8) && (UpdateMoveData(9) & bBlack))
      {
        //if not a promotion
        if (bCurrentPiece & NOTRANK7)
        {
          if (AddNextWhiteMove(Child,(&Child->bPawns)))
          {
            MoveNumber = 60; //Bookmark the next move to look at
            return(TRUE);
          }
        }
        else
        {
          MoveNumber++;
          if (GenerateNextWhitePawnPromotion(Child,bCurrentPiece >> 9))
            return(TRUE);
        }
      }

    case 6:
      //Capture To My Right (en passant): MoveNumber ==> 60
      if (bCurrentPiece & NOTFILE8)
      {
        if ((bCurrentPiece >> 1) & bEP)
        {
          Child->bFrom = bCurrentPiece;
          Child->From = CurrentPiece;
          Child->bAll &= ~Child->bFrom;
          Child->bWhite &= ~Child->bFrom;
          Child->bPawns &= ~Child->bFrom;

          Child->bTo = bCurrentPiece >> 9;
          Child->To = CurrentPiece + 9;
          Child->CaptureProcessing(bCurrentPiece >> 1);
          Child->bAll |= Child->bTo;
          Child->bWhite |= Child->bTo;
          Child->bPawns |= Child->bTo;
          Child->Capture = 1;
          Child->EPCapture = 1;
          Child->MustCheckSafety = 1;
          if (Child->DidWhiteJustMoveIntoCheck())
          {
            InitMyChild(Child);
          }
          else
          {
            if (Child->DidWhiteMoveCauseBlackCheck())
            {
              Child->BlackInCheck = ON;
            }
            Child->MovesSinceCapture = 0;
            Child->HashValue = (int) (Child->bAll % LARGE_PRIME);
            bCurrentPiece >>= 1;
            CurrentPiece++;
            MoveNumber = 10;
            return(TRUE);
          }
        }
      }

    default:
      bCurrentPiece >>= 1;
      CurrentPiece++;
      MoveNumber = 10;
      return(FALSE);
  }
}




//
//PERFORMANCE PATH
//
int Reset::GenerateNextBlackPawnMove(Reset *Child)
{
  switch (MoveNumber / 10)
  {
    case 1:
      //One space forward: MoveNumber ==> 10,11,12,13,14
      if (UpdateMoveData(-8) & ~bAll) 
      {
        //if not a promotion
        if (bCurrentPiece & NOTRANK2)
        {
          if (AddNextBlackMove(Child,(&Child->bPawns)))
          {
            MoveNumber = 20; //Bookmark the next move to look at
            return(TRUE);
          }
        }
        else
        {
          MoveNumber++;
          if (GenerateNextBlackPawnPromotion(Child,bCurrentPiece << 8))
            return(TRUE);
        }
      }

    case 2:
      //Two spaces forward: MoveNumber ==> 20
      if ((bCurrentPiece & RANK7) && 
          ((bCurrentPiece << 8) & ~bAll) &&
          (UpdateMoveData(-16) & ~bAll))
      {
        Child->bEP = bMoveData;
        if (AddNextBlackMove(Child,(&Child->bPawns)))
        {
          MoveNumber = 30; //Bookmark the next move to look at
          return(TRUE);
        }
      }
      MoveNumber = 30;

    case 3:
      //Capture To My Left (Normal): MoveNumber ==> 30,31,32,33,34
      if ((bCurrentPiece & NOTFILE1) && (UpdateMoveData(-9) & bWhite))
      {
        //if not a promotion
        if (bCurrentPiece & NOTRANK2)
        {
          if (AddNextBlackMove(Child,(&Child->bPawns)))
          {
            MoveNumber = 40; //Bookmark the next move to look at
            return(TRUE);
          }
        }
        else
        {
          MoveNumber++;
          if (GenerateNextBlackPawnPromotion(Child,bCurrentPiece << 9))
            return(TRUE);
        }
      }

    case 4:
      //Capture To My Left (en passant): MoveNumber ==> 40
      if (bCurrentPiece & NOTFILE1)
      {
        if ((bCurrentPiece << 1) & bEP)
        {
          Child->bFrom = bCurrentPiece;
          Child->From = CurrentPiece;
          Child->bAll &= ~Child->bFrom;
          Child->bBlack &= ~Child->bFrom;
          Child->bPawns &= ~Child->bFrom;

          Child->bTo = bCurrentPiece << 9;
          Child->To = CurrentPiece - 9;
          Child->CaptureProcessing(bCurrentPiece << 1);
          Child->bAll |= Child->bTo;
          Child->bBlack |= Child->bTo;
          Child->bPawns |= Child->bTo;
          Child->Capture = 1;
          Child->EPCapture = 1;
          Child->MustCheckSafety = 1;
          if (Child->DidBlackJustMoveIntoCheck())
          {
            InitMyChild(Child);
          }
          else
          {
            if (Child->DidBlackMoveCauseWhiteCheck())
            {
              Child->WhiteInCheck = ON;
            }
            Child->MovesSinceCapture = 0;
            Child->HashValue = (int) (Child->bAll % LARGE_PRIME);
            MoveNumber = 50; //Bookmark the next move to look at
            return(TRUE);
          }
        }
      }
      MoveNumber = 50;

    case 5:
      //Capture To My Right (Normal): MoveNumber ==> 50,51,52,53,54
      if ((bCurrentPiece & NOTFILE8) && (UpdateMoveData(-7) & bWhite))
      {
        //if not a promotion
        if (bCurrentPiece & NOTRANK2)
        {
          if (AddNextBlackMove(Child,(&Child->bPawns)))
          {
            MoveNumber = 60; //Bookmark the next move to look at
            return(TRUE);
          }
        }
        else
        {
          MoveNumber++;
          if (GenerateNextBlackPawnPromotion(Child,bCurrentPiece << 7))
            return(TRUE);
        }
      }

    case 6:
      //Capture To My Right (en passant): MoveNumber ==> 60
      if (bCurrentPiece & NOTFILE8)
      {
        if ((bCurrentPiece >> 1) & bEP)
        {
          Child->bFrom = bCurrentPiece;
          Child->From = CurrentPiece;
          Child->bAll &= ~Child->bFrom;
          Child->bBlack &= ~Child->bFrom;
          Child->bPawns &= ~Child->bFrom;

          Child->bTo = bCurrentPiece << 7;
          Child->To = CurrentPiece - 7;
          Child->CaptureProcessing(bCurrentPiece >> 1);
          Child->bAll |= Child->bTo;
          Child->bBlack |= Child->bTo;
          Child->bPawns |= Child->bTo;
          Child->Capture = 1;
          Child->EPCapture = 1;
          Child->MustCheckSafety = 1;
          if (Child->DidBlackJustMoveIntoCheck())
          {
            InitMyChild(Child);
          }
          else
          {
            if (Child->DidBlackMoveCauseWhiteCheck())
            {
              Child->WhiteInCheck = ON;
            }
            Child->MovesSinceCapture = 0;
            Child->HashValue = (int) (Child->bAll % LARGE_PRIME);
            bCurrentPiece >>= 1;
            CurrentPiece++;
            MoveNumber = 10;
            return(TRUE);
          }
        }
      }

    default:
      bCurrentPiece >>= 1;
      CurrentPiece++;
      MoveNumber = 10;
      return(FALSE);
  }
}





//
//PERFORMANCE PATH
//
int Reset::GenerateNextWhitePawnPromotion(Reset * Child, unsigned long long int Target)
{
  if (MoveNumber % 10 == 1)
  {
    //Knight
    Child->Promotion = 1;
    Child->bPawns &= ~bCurrentPiece;
    if (AddNextWhiteMove(Child,(&Child->bKnights)))
    {
      return(TRUE);
    }
    else
    {
      MoveNumber += 9;
      return(FALSE); //If one is false, all will be false
    }
  }
  

  if (MoveNumber % 10 == 2)
  {
    //Bishop
    Child->Promotion = 1;
    Child->bPawns &= ~bCurrentPiece;
    if (AddNextWhiteMove(Child,(&Child->bBishops)))
    if (ValidChild(Child))
    {
      return(TRUE);
    }
    else
      return(FALSE); //If one is false, all will be false
  }

  if (MoveNumber % 10 == 3)
  {
    //Rook
    Child->Promotion = 1;
    Child->bPawns &= ~bCurrentPiece;
    if (AddNextWhiteMove(Child,(&Child->bRooks)))
    {
      return(TRUE);
    }
    else
      return(FALSE); //If one is false, all will be false
  }

  if (MoveNumber % 10 == 4)
  {
    //Queen
    Child->Promotion = 1;
    Child->bPawns &= ~bCurrentPiece;
    if (AddNextWhiteMove(Child,(&Child->bQueens)))
    {
      MoveNumber += 6; //Bookmark the next move to look at
      return(TRUE);
    }
    else
      return(FALSE); //If one is false, all will be false
  }

}



//
//PERFORMANCE PATH
//
int Reset::GenerateNextBlackPawnPromotion(Reset *Child, unsigned long long int Target)
{
  if (MoveNumber % 10 == 1)
  {
    //Knight
    Child->Promotion = 1;
    Child->bPawns &= ~bCurrentPiece;
    if (AddNextBlackMove(Child,(&Child->bKnights)))
    {
      return(TRUE);
    }
    else
    {
      MoveNumber += 9;
      return(FALSE); //If one is false, all will be false
    }
  }

  if (MoveNumber % 10 == 2)
  {
    //Bishop
    Child->Promotion = 1;
    Child->bPawns &= ~bCurrentPiece;
    if (AddNextBlackMove(Child,(&Child->bBishops)))
    {
      return(TRUE);
    }
    else
      return(FALSE); //If one is false, all will be false
  }

  if (MoveNumber % 10 == 3)
  {
    //Rook
    Child->Promotion = 1;
    Child->bPawns &= ~bCurrentPiece;
    if (AddNextBlackMove(Child,(&Child->bRooks)))
    {
      return(TRUE);
    }
    else
      return(FALSE); //If one is false, all will be false
  }

  if (MoveNumber % 10 == 4)
  {
    //Queen
    Child->Promotion = 1;
    Child->bPawns &= ~bCurrentPiece;
    if (AddNextBlackMove(Child,(&Child->bQueens)))
    {
      MoveNumber += 6; //Bookmark the next move to look at
      return(TRUE);
    }
    else
      return(FALSE); //If one is false, all will be false
  }

}






int Reset::GenerateNextWhitePawnCapture(Reset *Child)
{
  switch (MoveNumber / 10)
  {
    case 1:
      //Capture To My Left (Normal): MoveNumber ==> 10,11,12,13,14
      if ((bCurrentPiece & NOTFILE1) && (UpdateMoveData(7) & bBlack))
      {
        //if not a promotion
        if (bCurrentPiece & NOTRANK7)
        {
          if (AddNextWhiteMove(Child,(&Child->bPawns)))
          {
            MoveNumber = 40; //Bookmark the next move to look at
            return(TRUE);
          }
        }
        else
        {
          MoveNumber++;
          if (GenerateNextWhitePawnPromotion(Child,bCurrentPiece >> 7))
            return(TRUE);
        }
      }

    case 4:
      //Capture To My Left (en passant): MoveNumber ==> 40
      if (bCurrentPiece & NOTFILE1)
      {
        if ((bCurrentPiece << 1) & bEP)
        {
          Child->bFrom = bCurrentPiece;
          Child->From = CurrentPiece;
          Child->bAll &= ~Child->bFrom;
          Child->bWhite &= ~Child->bFrom;
          Child->bPawns &= ~Child->bFrom;

          Child->bTo = bCurrentPiece >> 7;
          Child->To = CurrentPiece + 7;
          Child->CaptureProcessing(bCurrentPiece << 1);
          Child->bAll |= Child->bTo;
          Child->bWhite |= Child->bTo;
          Child->bPawns |= Child->bTo;
          Child->Capture = 1;
          Child->EPCapture = 1;
          Child->MustCheckSafety = 1;
          if (Child->DidWhiteJustMoveIntoCheck())
          {
            InitMyChild(Child);
          }
          else
          {
            if (Child->DidWhiteMoveCauseBlackCheck())
            {
              Child->BlackInCheck = ON;
            }
            Child->HashValue = (int) (Child->bAll % LARGE_PRIME);
            MoveNumber = 50; //Bookmark the next move to look at
            return(TRUE);
          }
        }
      }
      MoveNumber = 50;

    case 5:
      //Capture To My Right (Normal): MoveNumber ==> 50,51,52,53,54
      if ((bCurrentPiece & NOTFILE8) && (UpdateMoveData(9) & bBlack))
      {
        //if not a promotion
        if (bCurrentPiece & NOTRANK7)
        {
          if (AddNextWhiteMove(Child,(&Child->bPawns)))
          {
            MoveNumber = 60; //Bookmark the next move to look at
            return(TRUE);
          }
        }
        else
        {
          MoveNumber++;
          if (GenerateNextWhitePawnPromotion(Child,bCurrentPiece >> 9))
            return(TRUE);
        }
      }

    case 6:
      //Capture To My Right (en passant): MoveNumber ==> 60
      if (bCurrentPiece & NOTFILE8)
      {
        if ((bCurrentPiece >> 1) & bEP)
        {
          Child->bFrom = bCurrentPiece;
          Child->From = CurrentPiece;
          Child->bAll &= ~Child->bFrom;
          Child->bWhite &= ~Child->bFrom;
          Child->bPawns &= ~Child->bFrom;

          Child->bTo = bCurrentPiece >> 9;
          Child->To = CurrentPiece + 9;
          Child->CaptureProcessing(bCurrentPiece >> 1);
          Child->bAll |= Child->bTo;
          Child->bWhite |= Child->bTo;
          Child->bPawns |= Child->bTo;
          Child->Capture = 1;
          Child->EPCapture = 1;
          Child->MustCheckSafety = 1;
          if (Child->DidWhiteJustMoveIntoCheck())
          {
            InitMyChild(Child);
          }
          else
          {
            if (Child->DidWhiteMoveCauseBlackCheck())
            {
              Child->BlackInCheck = ON;
            }
            Child->HashValue = (int) (Child->bAll % LARGE_PRIME);
            bCurrentPiece >>= 1;
            CurrentPiece++;
            MoveNumber = 10;
            return(TRUE);
          }
        }
      }

    default:
      bCurrentPiece >>= 1;
      CurrentPiece++;
      MoveNumber = 10;
      return(FALSE);
  }
}




int Reset::GenerateNextBlackPawnCapture(Reset *Child)
{
  switch (MoveNumber / 10)
  {
    case 1:
      //Capture To My Left (Normal): MoveNumber ==> 10,11,12,13,14
      if ((bCurrentPiece & NOTFILE1) && (UpdateMoveData(-9) & bWhite))
      {
        //if not a promotion
        if (bCurrentPiece & NOTRANK2)
        {
          if (AddNextBlackMove(Child,(&Child->bPawns)))
          {
            MoveNumber = 40; //Bookmark the next move to look at
            return(TRUE);
          }
        }
        else
        {
          MoveNumber++;
          if (GenerateNextBlackPawnPromotion(Child,bCurrentPiece << 9))
            return(TRUE);
        }
      }

    case 4:
      //Capture To My Left (en passant): MoveNumber ==> 40
      if (bCurrentPiece & NOTFILE1)
      {
        if ((bCurrentPiece << 1) & bEP)
        {
          Child->bFrom = bCurrentPiece;
          Child->From = CurrentPiece;
          Child->bAll &= ~Child->bFrom;
          Child->bBlack &= ~Child->bFrom;
          Child->bPawns &= ~Child->bFrom;

          Child->bTo = bCurrentPiece << 9;
          Child->To = CurrentPiece - 9;
          Child->CaptureProcessing(bCurrentPiece << 1);
          Child->bAll |= Child->bTo;
          Child->bBlack |= Child->bTo;
          Child->bPawns |= Child->bTo;
          Child->Capture = 1;
          Child->EPCapture = 1;
          Child->MustCheckSafety = 1;
          if (Child->DidBlackJustMoveIntoCheck())
          {
            InitMyChild(Child);
          }
          else
          {
            if (Child->DidBlackMoveCauseWhiteCheck())
            {
              Child->WhiteInCheck = ON;
            }
            Child->HashValue = (int) (Child->bAll % LARGE_PRIME);
            MoveNumber = 50; //Bookmark the next move to look at
            return(TRUE);
          }
        }
      }
      MoveNumber = 50;

    case 5:
      //Capture To My Right (Normal): MoveNumber ==> 50,51,52,53,54
      if ((bCurrentPiece & NOTFILE8) && (UpdateMoveData(-7) & bWhite))
      {
        //if not a promotion
        if (bCurrentPiece & NOTRANK2)
        {
          if (AddNextBlackMove(Child,(&Child->bPawns)))
          {
            MoveNumber = 60; //Bookmark the next move to look at
            return(TRUE);
          }
        }
        else
        {
          MoveNumber++;
          if (GenerateNextBlackPawnPromotion(Child,bCurrentPiece << 7))
            return(TRUE);
        }
      }

    case 6:
      //Capture To My Right (en passant): MoveNumber ==> 60
      if (bCurrentPiece & NOTFILE8)
      {
        if ((bCurrentPiece >> 1) & bEP)
        {
          Child->bFrom = bCurrentPiece;
          Child->From = CurrentPiece;
          Child->bAll &= ~Child->bFrom;
          Child->bBlack &= ~Child->bFrom;
          Child->bPawns &= ~Child->bFrom;

          Child->bTo = bCurrentPiece << 7;
          Child->To = CurrentPiece - 7;
          Child->CaptureProcessing(bCurrentPiece >> 1);
          Child->bAll |= Child->bTo;
          Child->bBlack |= Child->bTo;
          Child->bPawns |= Child->bTo;
          Child->Capture = 1;
          Child->EPCapture = 1;
          Child->MustCheckSafety = 1;
          if (Child->DidBlackJustMoveIntoCheck())
          {
            InitMyChild(Child);
          }
          else
          {
            if (Child->DidBlackMoveCauseWhiteCheck())
            {
              Child->WhiteInCheck = ON;
            }
            Child->HashValue = (int) (Child->bAll % LARGE_PRIME);
            bCurrentPiece >>= 1;
            CurrentPiece++;
            MoveNumber = 10;
            return(TRUE);
          }
        }
      }

    default:
      bCurrentPiece >>= 1;
      CurrentPiece++;
      MoveNumber = 10;
      return(FALSE);
  }
}





