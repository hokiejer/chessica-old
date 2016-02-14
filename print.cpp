#include "chess.h"

#define CLEAR_SCREEN system("clear\n")
#define INVERSE_TEXT "\033[7m"
#define BOLD_TEXT "\033[1m"
#define NORMAL_TEXT "\033[0m"
#define DARK_GRAY_TEXT "\033[1;30m"
#define LIGHT_GRAY_TEXT "\033[0;37m"
#define BLACK_ON_YELLOW_TEXT "\033[1;33;40m"
#define WHITE_TEXT "\033[1;37m"


void Reset::PrintSquare(int SquareIndex, int Level)
{
  int SquareColor;
  unsigned long long int Mask = 0x8000000000000000;

  if (((SquareIndex / 8) + (SquareIndex % 8)) % 2 == 0)
    SquareColor = BLACK;
  else
    SquareColor = WHITE;
  
  if (Level != 2)
  {
    if (SquareColor == BLACK)
    {
      printf(INVERSE_TEXT);
      printf(DARK_GRAY_TEXT);
      printf("       ");
      printf(NORMAL_TEXT);
    }
    else
      printf("       ");
  }
  else
  {
    if (SquareColor == BLACK)
    {
      printf(INVERSE_TEXT);
      printf(DARK_GRAY_TEXT);
      printf("  ");
      printf(NORMAL_TEXT);
    }
    else
      printf("  ");

    if ((Mask >> SquareIndex) & bAll)
    {
      if ((Mask >> SquareIndex) & bWhite)
      {
        printf(INVERSE_TEXT);
        printf(BLACK_ON_YELLOW_TEXT);
        printf(BOLD_TEXT);
        if ((Mask >> SquareIndex) & bPawns)
          printf(" P ");
        if ((Mask >> SquareIndex) & bKnights)
          printf(" N ");
        if ((Mask >> SquareIndex) & bBishops)
          printf(" B ");
        if ((Mask >> SquareIndex) & bRooks)
          printf(" R ");
        if ((Mask >> SquareIndex) & bQueens)
          printf(" Q ");
        if ((Mask >> SquareIndex) & bKings)
          printf(" K ");
        printf(NORMAL_TEXT);
      }
      else /* Piece color is Black */
      {
        printf(INVERSE_TEXT);
        printf(BOLD_TEXT);
        if ((Mask >> SquareIndex) & bPawns)
          printf(" p ");
        if ((Mask >> SquareIndex) & bKnights)
          printf(" n ");
        if ((Mask >> SquareIndex) & bBishops)
          printf(" b ");
        if ((Mask >> SquareIndex) & bRooks)
          printf(" r ");
        if ((Mask >> SquareIndex) & bQueens)
          printf(" q ");
        if ((Mask >> SquareIndex) & bKings)
          printf(" k ");
        printf(NORMAL_TEXT);
      }
      if (SquareColor == BLACK)
      {
        printf(INVERSE_TEXT);
        printf(DARK_GRAY_TEXT);
        printf("  ");
        printf(NORMAL_TEXT);
      }
      else
        printf("  ");
    }
    else
    {
      if (SquareColor == BLACK)
      {
        printf(INVERSE_TEXT);
        printf(DARK_GRAY_TEXT);
        printf("     ");
        printf(NORMAL_TEXT);
      }
      else
        printf("     ");
    }
  }
};


void Reset::PrintBoardRow(int Start, int End, int Name)
{
  int level, loop;
  for (level=1;level<=3;level++)
  {
    printf(INVERSE_TEXT);
    if (level == 2)
      printf(" %d- ",Name);
    else
      printf("    ");
    printf(NORMAL_TEXT);

    for ( loop = Start; loop < End; ++loop )
      PrintSquare(loop,level);

    printf(INVERSE_TEXT);
    if (level == 2)
      printf(" -%d ",Name);
    else
      printf("    ");
    printf(NORMAL_TEXT);
    printf("\n");
  }
}


/* procedure PrintBoard prints the chessboard. */
void Reset::PrintBoard()
{
  int loop, level;

  printf(INVERSE_TEXT);
  printf("       a      b      c      d      e      f      g      h       ");
  printf(NORMAL_TEXT);
  printf("\n");

  printf(INVERSE_TEXT);
  printf("       |      |      |      |      |      |      |      |       ");
  printf(NORMAL_TEXT);
  printf("\n");

  PrintBoardRow(56,64,8);
  PrintBoardRow(48,56,7);
  PrintBoardRow(40,48,6);
  PrintBoardRow(32,40,5);
  PrintBoardRow(24,32,4);
  PrintBoardRow(16,24,3);
  PrintBoardRow(8,16,2);
  PrintBoardRow(0,8,1);

  printf(INVERSE_TEXT);
  printf("       |      |      |      |      |      |      |      |       ");
  printf(NORMAL_TEXT);
  printf("\n");

  printf(INVERSE_TEXT);
  printf("       a      b      c      d      e      f      g      h       ");
  printf(NORMAL_TEXT);
  printf("\n");

};





//void Reset::LogPrintSquare(signed char PieceIndex)
//{
  //if (PieceIndex == EMPTY)
    //fprintf(logfile,". ");
  //else
  //{
    //if (PIECEISBLACK(PieceIndex))
    //{
      //switch(PieceType[PieceIndex])
      //{
        //case PAWN: fprintf(logfile,"p ");
                   //break;
        //case KNIGHT: fprintf(logfile,"n ");
                     //break;
        //case BISHOP: fprintf(logfile,"b ");
                     //break;
        //case ROOK: fprintf(logfile,"r ");
                   //break;
        //case QUEEN: fprintf(logfile,"q ");
                    //break;
        ////case KING: fprintf(logfile,"k ");
                   //break;
        //default: fprintf(logfile,"? ");
      //}
    //}
    //else /* Piece color is White */
    //{
      //switch(PieceType[PieceIndex])
      //{
        //case PAWN: fprintf(logfile,"P ");
                   //break;
        //case KNIGHT: fprintf(logfile,"N ");
                     //break;
        //case BISHOP: fprintf(logfile,"B ");
                     //break;
        //case ROOK: fprintf(logfile,"R ");
                   //break;
        //case QUEEN: fprintf(logfile,"Q ");
                    //break;
        //case KING: fprintf(logfile,"K ");
                   //break;
        //default: fprintf(logfile,"? ");
      //}
    //}
   // 
  //}
//};




//void Reset::LogPrintBoard()
//{
  //int loop;
//
  //fprintf(logfile,"\n   a b c d e f g h \n\n");
  //fprintf(logfile,"8  ");
//
  //for ( loop = 56; loop < 64; ++loop )
    //LogPrintSquare(Board[loop]);
  //fprintf(logfile," 8\n7  ");
//
  //for ( loop = 48; loop < 56; ++loop )
    //LogPrintSquare(Board[loop]);
  //fprintf(logfile," 7\n6  ");
//
  //for ( loop = 40; loop < 48; ++loop )
    //LogPrintSquare(Board[loop]);
  //fprintf(logfile," 6\n5  ");
//
  //for ( loop = 32; loop < 40; ++loop )
    //LogPrintSquare(Board[loop]);
  //fprintf(logfile," 5\n4  ");
//
  //for ( loop = 24; loop < 32; ++loop )
    //LogPrintSquare(Board[loop]);
  //fprintf(logfile," 4\n3  ");
//
  //for ( loop = 16; loop < 24; ++loop )
    //LogPrintSquare(Board[loop]);
  //fprintf(logfile," 3\n2  ");
//
  //for ( loop = 8; loop < 16; ++loop )
    //LogPrintSquare(Board[loop]);
  //fprintf(logfile," 2\n1  ");
//
  //for ( loop = 0; loop < 8; ++loop )
    //LogPrintSquare(Board[loop]);
  //fprintf(logfile," 1\n\n");
  //fprintf(logfile,"   a b c d e f g h\n\n");
//};





void Reset::PrintSquareToAddress(unsigned long long int Mask, char *Addr)
{
  if (Mask & bAll)
  {
    if (Mask & bWhite)
    {
      if (Mask & bPawns)
        *Addr = 'P';
      if (Mask & bBishops)
        *Addr = 'B';
      if (Mask & bKnights)
        *Addr = 'N';
      if (Mask & bRooks)
        *Addr = 'R';
      if (Mask & bQueens)
        *Addr = 'Q';
      if (Mask & bKings)
        *Addr = 'K';
    }
    else //Black
    {
      if (Mask & bPawns)
        *Addr = 'p';
      if (Mask & bBishops)
        *Addr = 'b';
      if (Mask & bKnights)
        *Addr = 'n';
      if (Mask & bRooks)
        *Addr = 'r';
      if (Mask & bQueens)
        *Addr = 'q';
      if (Mask & bKings)
        *Addr = 'k';
    }
  }
  else
  {
    sprintf(Addr,".");
  }
};


void Reset::PrintMoveHistory(int Ply)
{
  char Boards[8][8][8]; //Board,Row,Column
  int i,j,k;
  unsigned long long int Mask;
  int iterator;

  for (i=0;i<=Ply;i++)
  {
    Mask = 0x8000000000000000;
    for (j=0;j<64;j++)
    {
      MoveHistory[i]->PrintSquareToAddress(Mask, &(Boards[i][j/8][j%8]));
      Mask >>= 1;
    }
  }
  for (i=7;i>=0;i--)
  {
    for (j=0;j<=Ply;j++)
    {
      for (k=0;k<8;k++)
      {
        printf("%c ",Boards[j][i][k]);
      }
      printf("    ");
    }
    printf("\n");
  }
  printf("\n");
}


void Reset::PrintSmallBoard()
{
  char Boards[8][8]; //Board,Row,Column
  int i,j,k;
  unsigned long long int Mask;
  int iterator;

  Mask = 0x8000000000000000;
  for (j=0;j<64;j++)
  {
    PrintSquareToAddress(Mask, &(Boards[j/8][j%8]));
    Mask >>= 1;
  }
  for (i=7;i>=0;i--)
  {
    for (k=0;k<8;k++)
    {
      printf("%c ",Boards[i][k]);
    }
    printf("\n");
  }
  printf("\n");
}


void Reset::PrintReset()
{
  char FromText[3];
  char ToText[3];
  unsigned long int blah;
  //float TheScore = ScoreBoard(ptr,&blah);

  SquareNumberToText(From,FromText);
  SquareNumberToText(To,ToText);
  PrintBoard();
  printf("Last move from %s to %s.",FromText,ToText);
  if (WhiteInCheck)
  {
    printf(" WHITE IN CHECK!");
  }
  if (BlackInCheck)
  {
    printf(" BLACK IN CHECK!");
  }
  if (ToMove == WHITE)
  {
    printf(" White to move next.");
  }
  else
  {
    printf(" Black to move next.");
  }
  printf("\n");
  printf("Board score: %f (%d)  Hash Value: %d\n",Score,ScoreDepth,HashValue);
  if (WhiteCastleQ) printf("Q");
  if (WhiteCastleK) printf("K");
  if (BlackCastleQ) printf("q");
  if (BlackCastleK) printf("k");
  if (bEP) printf(" bEP:%016llX",bEP);
printf("\n");
printf("bAll = %016llX\n",bAll);
printf("bWhite = %016llX  ",bWhite);
printf("bBlack = %016llX\n",bBlack);
printf("bPawns = %016llX  ",bPawns);
printf("bKings = %016llX  ",bKings);
printf("bKnights = %016llX  ",bKnights);
printf("bBishops = %016llX\n",bBishops);
printf("bRooks = %016llX  ",bRooks);
printf("bQueens = %016llX  \n",bQueens);
printf("bCurrentPiece = %016llX  MoveNumber=%d",bCurrentPiece,MoveNumber);
  printf("\n\n");
  //getchar();
}

void ResetTree::PrintChildren()
{
  ResetTree *CurrentNode = Children.First;	//Start with the first child

  while (CurrentNode)				//While there are children
  {
    CurrentNode->PrintReset();			//Print this child
    CurrentNode = CurrentNode->NextSibling;
  }
}


void ClearScreen()
{
  CLEAR_SCREEN;
}


void ResetTree::PrintBestMoveLine()
{
  char mybuffer[100];

  printf("%s",PrintBestMoveLineToBuffer(mybuffer));
}



char *ResetTree::PrintBestMoveLineToBuffer(char *outbuffer)
{
  char mybuffer[10];
  ResetTree *printptr;

  printptr = Children.First;
  while (printptr)
  {
    printptr->GetAlgebraicNotation(printptr->Parent,mybuffer);
    sprintf(outbuffer,"%s  ",mybuffer);
    printptr = printptr->Children.First;
  }
  sprintf(outbuffer,"\n");
  return(outbuffer);
}
