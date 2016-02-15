#include "chess.h"

void Reset::TestSafety(int Color, unsigned long long int SafeSquares)
{
  unsigned long long int Mask = 0x8000000000000000;
  unsigned long long int MyResults = 0;

  while (Mask)
  {
    if (Color == WHITE)
    {
      if (WhiteIsSafe(Mask))
        MyResults |= Mask;
    }
    else
    {
      if (BlackIsSafe(Mask))
        MyResults |= Mask;
    }
    Mask >>= 1;
  }
  
  printf("Testing Safety: Goal == %016llX, Computed == %016llX",SafeSquares,MyResults);
  if (MyResults != SafeSquares)
    printf("  ERROR!");
  printf("\n");
}

