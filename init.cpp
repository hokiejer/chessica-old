#include "chess.h"

void Reset::bInitToFEN(const char *FEN)
{
  signed char square = 56;
  unsigned long long int Mask = 0x8000000000000000;
  int i = 0;
  char inchar;
  int param = 0;
  char SquareText[2];

  //printf("FEN: %s\n",FEN);
  bAll = 0;
  bWhite = 0;
  bBlack = 0;
  bPawns = 0;
  bKnights = 0;
  bBishops = 0;
  bRooks = 0;
  bQueens = 0;
  bKings = 0;
  bEP = 0;
  WhiteCastleQ = 0;
  WhiteCastleK = 0;
  BlackCastleQ = 0;
  BlackCastleK = 0;
  Material = 0;
  EPCapture = 1;  //This will force Safety checks to work on FEN-initialized resets
  MovesSinceCapture = 0;
  i = 0;

  while (inchar = FEN[i++])
  {
    switch(param)
    {
      case 0:
        switch (inchar)
        {
          case '8':
            square++;
          case '7':
            square++;
          case '6':
            square++;
          case '5':
            square++;
          case '4':
            square++;
          case '3':
            square++;
          case '2':
            square++;
          case '1':
            square++;
            break;
          case 'p':
            bAll |= Mask >> square;
            bBlack |= Mask >> square;
            bPawns |= Mask >> square;
            square++;
            break;
          case 'n':
            bAll |= Mask >> square;
            bBlack |= Mask >> square;
            bKnights |= Mask >> square;
            square++;
            break;
          case 'b':
            bAll |= Mask >> square;
            bBlack |= Mask >> square;
            bBishops |= Mask >> square;
            square++;
            break;
          case 'r':
            bAll |= Mask >> square;
            bBlack |= Mask >> square;
            bRooks |= Mask >> square;
            square++;
            break;
          case 'q':
            bAll |= Mask >> square;
            bBlack |= Mask >> square;
            bQueens |= Mask >> square;
            square++;
            break;
          case 'k':
            bAll |= Mask >> square;
            bBlack |= Mask >> square;
            bKings |= Mask >> square;
            BlackKingSquare = square;
            square++;
            break;
          case 'P':
            bAll |= Mask >> square;
            bWhite |= Mask >> square;
            bPawns |= Mask >> square;
            square++;
            break;
          case 'N':
            bAll |= Mask >> square;
            bWhite |= Mask >> square;
            bKnights |= Mask >> square;
            square++;
            break;
          case 'B':
            bAll |= Mask >> square;
            bWhite |= Mask >> square;
            bBishops |= Mask >> square;
            square++;
            break;
          case 'R':
            bAll |= Mask >> square;
            bWhite |= Mask >> square;
            bRooks |= Mask >> square;
            square++;
            break;
          case 'Q':
            bAll |= Mask >> square;
            bWhite |= Mask >> square;
            bQueens |= Mask >> square;
            square++;
            break;
          case 'K':
            bAll |= Mask >> square;
            bWhite |= Mask >> square;
            bKings |= Mask >> square;
            WhiteKingSquare = square;
            square++;
            break;
          case '/':
            square -= 16;
            break;
          case ' ':
            param++;
            break;
        }
        break;
      case 1:
        switch (inchar)
        {
          case 'w':
            ToMove = WHITE;
            break;
          case 'b':
            ToMove = BLACK;
            break;
          case ' ':
            param++;
            break;
        }
        break;
      case 2:
        switch (inchar)
        {
          case 'q':
            BlackCastleQ = 1;
            break;
          case 'k':
            BlackCastleK = 1;
            break;
          case 'Q':
            WhiteCastleQ = 1;
            break;
          case 'K':
            WhiteCastleK = 1;
            break;
          case '-':
            break;
          case ' ':
            param++;
            break;
        }
        break;
      case 3:
        //I store EP as the space the pawn moved to, this is the space skipped
        switch (inchar)
        {
          case 'a':
          case 'b':
          case 'c':
          case 'd':
          case 'e':
          case 'f':
          case 'g':
          case 'h':
            SquareText[0] = inchar;
            break;
          case '3':
            SquareText[1] = inchar;
            bEP = Mask >> (TextToSquareNumber(SquareText) + 8); //white
            param++;
            break;
          case '6':
            SquareText[1] = inchar;
            bEP = Mask >> (TextToSquareNumber(SquareText) - 8); //black
            param++;
            break;
          case '-':
            param++;
            break;
        }
        break;
      case 4:
        switch (inchar)
        {
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            MovesSinceCapture = (unsigned char) ((MovesSinceCapture * 10) + (inchar - '0'));
            break;
          case ' ':
            param++;
            break;
        }
        break;
    }
    // I am ignoring the halfmove clock (4) and fullmove number (5)
  }

  Material = 0;
  bFrom = 0;
  bTo = 0;
  HashValue = (int) (bAll & LARGE_PRIME);
  if (WhiteIsSafe(bWhite & bKings))
    WhiteInCheck = OFF;
  else
    WhiteInCheck = ON;
  if (BlackIsSafe(bBlack & bKings))
    BlackInCheck = OFF;
  else
    BlackInCheck = ON;
};


