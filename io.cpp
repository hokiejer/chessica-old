#include "chess.h"

Lock IOLock;

int TextToSquareNumber(char Text[])
{
  int returnval = 0;
  switch(Text[0]) {
    case 'a': returnval += 0; break;
    case 'b': returnval += 1; break;
    case 'c': returnval += 2; break;
    case 'd': returnval += 3; break;
    case 'e': returnval += 4; break;
    case 'f': returnval += 5; break;
    case 'g': returnval += 6; break;
    case 'h': returnval += 7; break;
    default: return(-1);
  }
  switch(Text[1]) {
    case '1': returnval += 0;  break;
    case '2': returnval += 8;  break;
    case '3': returnval += 16; break;
    case '4': returnval += 24; break;
    case '5': returnval += 32; break;
    case '6': returnval += 40; break;
    case '7': returnval += 48; break;
    case '8': returnval += 56; break;
    default: return(-1);
  }
  return(returnval);
}

int SquareNumberToText(int SquareNumber, char Text[])
{
  int returnval = 0;

  switch(SquareNumber % 8) {
    case 0: Text[0] = 'a'; break;  
    case 1: Text[0] = 'b'; break;  
    case 2: Text[0] = 'c'; break;  
    case 3: Text[0] = 'd'; break;  
    case 4: Text[0] = 'e'; break;  
    case 5: Text[0] = 'f'; break;  
    case 6: Text[0] = 'g'; break;  
    case 7: Text[0] = 'h'; break;  
    default: return(-1);
  }
  switch(SquareNumber / 8) {
    case 0: Text[1] = '1'; break;  
    case 1: Text[1] = '2'; break;  
    case 2: Text[1] = '3'; break;  
    case 3: Text[1] = '4'; break;  
    case 4: Text[1] = '5'; break;  
    case 5: Text[1] = '6'; break;  
    case 6: Text[1] = '7'; break;  
    case 7: Text[1] = '8'; break;  
    default: return(-1);
  }
  Text[2] = '\0';
  return(returnval);
}


void Reset::GetAlgebraicNotation(Reset *Parent, char Text[])
{
  char tobuffer[5], frombuffer[5], tempbuffer[5];
  int i = 0;
  int DisambiguateFile = 0; //File is a column (letter)
  int DisambiguateRank = 0; //Rank is a row (number)
  Reset LP, LC;
  Reset *LocalParent = &LP;
  Reset *LocalChild = &LC;
  

  if (KingCastled)
  {
    Text[i++] = 'O';
    Text[i++] = '-';
    Text[i++] = 'O';
    if ((To == 2) || (To == 58))
    {
      Text[i++] = '-';
      Text[i++] = 'O';
    }
  }
  else
  {
    Parent->CopyReset(LocalParent);
    SquareNumberToText(To,tobuffer);
    SquareNumberToText(From,frombuffer);
    LocalParent->InitializeMoveGeneration();
    LocalParent->InitMyChild(LocalChild);

    if (Promotion || EPCapture || (bPawns & bTo))
    {
      if (Capture || EPCapture)
      {
        Text[i++] = frombuffer[0];
        Text[i++] = 'x';
      }
      Text[i++] = tobuffer[0];
      Text[i++] = tobuffer[1];

      if (Promotion)
      {
        if (bTo & bKnights)
          Text[i++] = 'N';
        if (bTo & bBishops)
          Text[i++] = 'B';
        if (bTo & bRooks)
          Text[i++] = 'R';
        if (bTo & bQueens)
          Text[i++] = 'Q';
      }
    }


    if (bKnights & bTo)
    {
      while(LocalParent->GenerateNextMove(LocalChild))
      {
        if (!ResetMatches(LocalChild))
        {
          if (bTo & LocalChild->bTo & LocalChild->bKnights)
          {
            SquareNumberToText(LocalChild->From,tempbuffer);
            if (tempbuffer[0] == frombuffer[0])
              DisambiguateRank = 1;
            else
            {
              if (tempbuffer[1] == frombuffer[1])
                DisambiguateFile = 1;
            }
          }
        }
        LocalParent->InitMyChild(LocalChild);
      }

      Text[i++] = 'N';
      if (DisambiguateFile)
        Text[i++] = frombuffer[0];
      if (DisambiguateRank)
        Text[i++] = frombuffer[1];
      if (Capture)
        Text[i++] = 'x';
      Text[i++] = tobuffer[0];
      Text[i++] = tobuffer[1];
    }

    if (bBishops & bTo)
    {
      while(LocalParent->GenerateNextMove(LocalChild))
      {
        if (!ResetMatches(LocalChild))
        {
          if (bTo & LocalChild->bTo & LocalChild->bBishops)
          {
            SquareNumberToText(LocalChild->From,tempbuffer);
            if (tempbuffer[0] == frombuffer[0])
              DisambiguateRank = 1;
            else
            {
              if (tempbuffer[1] == frombuffer[1])
                DisambiguateFile = 1;
            }
          }
        }
        LocalParent->InitMyChild(LocalChild);
      }

      Text[i++] = 'B';
      if (DisambiguateFile)
        Text[i++] = frombuffer[0];
      if (DisambiguateRank)
        Text[i++] = frombuffer[1];
      if (Capture)
        Text[i++] = 'x';
      Text[i++] = tobuffer[0];
      Text[i++] = tobuffer[1];
    }

    if (bRooks & bTo)
    {
      while(LocalParent->GenerateNextMove(LocalChild))
      {
        if (!ResetMatches(LocalChild))
        {
          if (bTo & LocalChild->bTo & LocalChild->bRooks)
          {
            SquareNumberToText(LocalChild->From,tempbuffer);
            if (tempbuffer[0] == frombuffer[0])
              DisambiguateRank = 1;
            else
            {
              if (tempbuffer[1] == frombuffer[1])
                DisambiguateFile = 1;
            }
          }
        }
        LocalParent->InitMyChild(LocalChild);
      }

      Text[i++] = 'R';
      if (DisambiguateFile)
        Text[i++] = frombuffer[0];
      if (DisambiguateRank)
        Text[i++] = frombuffer[1];
      if (Capture)
        Text[i++] = 'x';
      Text[i++] = tobuffer[0];
      Text[i++] = tobuffer[1];
    }

    if (bQueens & bTo)
    {
      while(LocalParent->GenerateNextMove(LocalChild))
      {
        if (!ResetMatches(LocalChild))
        {
          if (bTo & LocalChild->bTo & LocalChild->bQueens)
          {
            SquareNumberToText(LocalChild->From,tempbuffer);
            if (tempbuffer[0] == frombuffer[0])
              DisambiguateRank = 1;
            else
            {
              if (tempbuffer[1] == frombuffer[1])
                DisambiguateFile = 1;
            }
          }
        }
        LocalParent->InitMyChild(LocalChild);
      }

      Text[i++] = 'Q';
      if (DisambiguateFile)
        Text[i++] = frombuffer[0];
      if (DisambiguateRank)
        Text[i++] = frombuffer[1];
      if (Capture)
        Text[i++] = 'x';
      Text[i++] = tobuffer[0];
      Text[i++] = tobuffer[1];
    }

    if (bKings & bTo)
    {
      Text[i++] = 'K';
      if (Capture)
        Text[i++] = 'x';
      Text[i++] = tobuffer[0];
      Text[i++] = tobuffer[1];
    }
  }

  if (WhiteInCheck || BlackInCheck)
  {
    if (GameOver)
      Text[i++] = '#';
    else
      Text[i++] = '+';
  }
  Text[i++] = '\0';
}

