#include "chess.h"

#define NEWERWAY 1

#define SAFE 0
#define LOOKUP 1
#define LOOKUPRIGHT 2
#define LOOKRIGHT 3
#define LOOKDOWNRIGHT 4
#define LOOKDOWN 5
#define LOOKDOWNLEFT 6
#define LOOKLEFT 7
#define LOOKUPLEFT 8
#define LOOKKNIGHT 9

#define CALL_MEMBER_FUNCTION(object,memberfunction) ((object).*(memberfunction))

unsigned long long int KingStars[64];
unsigned long long int AllAttacks[64];
typedef int (Reset::*ISSAFEMETHOD) (void);
ISSAFEMETHOD WhiteIsSafeFromRevealedCheck[64][64]; //[King][Square]
ISSAFEMETHOD BlackIsSafeFromRevealedCheck[64][64]; //[King][Square]
ISSAFEMETHOD WhiteIsSafeFromCheck[64][64]; //[King][Square]
ISSAFEMETHOD BlackIsSafeFromCheck[64][64]; //[King][Square]
signed char WhiteIsSafeFromRevealedCheckIndex[64][64];
signed char BlackIsSafeFromRevealedCheckIndex[64][64];
signed char WhiteIsSafeFromDirectCheckIndex[64][64];
signed char BlackIsSafeFromDirectCheckIndex[64][64];

void Reset::InitCheckFunctionRouters()
{
  int i, j, Temp;
  unsigned long long int BitMask = 0x8000000000000000;
  unsigned long long int i_Bit, j_Bit, bTemp;

  for (i=0;i<64;i++) //For each king square
  {
    for (j=0;j<64;j++) //For each target square
    {
      WhiteIsSafeFromRevealedCheck[i][j] = & Reset::SafeFromCheck;
      BlackIsSafeFromRevealedCheck[i][j] = & Reset::SafeFromCheck;
      WhiteIsSafeFromCheck[i][j] = & Reset::SafeFromCheck;
      BlackIsSafeFromCheck[i][j] = & Reset::SafeFromCheck;
      WhiteIsSafeFromRevealedCheckIndex[i][j] = SAFE;
      BlackIsSafeFromRevealedCheckIndex[i][j] = SAFE;
      WhiteIsSafeFromDirectCheckIndex[i][j] = SAFE;
      BlackIsSafeFromDirectCheckIndex[i][j] = SAFE;
    }
  }

  for (i=0;i<64;i++) //For each king square
  {
    i_Bit = BitMask >> i;

  
    /* Bishop or Queen */
    bTemp = i_Bit;
    j = i;
    while(bTemp & ULEDGE)
    {
      bTemp = (bTemp & ULEDGE) >> 7;
      j += 7;
      WhiteIsSafeFromRevealedCheck[i][j] = & Reset::WhiteIsSafeFromRevealedCheckUpLeft;
      BlackIsSafeFromRevealedCheck[i][j] = & Reset::BlackIsSafeFromRevealedCheckUpLeft;
      WhiteIsSafeFromCheck[i][j] = & Reset::WhiteIsSafeFromCheckUpLeft;
      BlackIsSafeFromCheck[i][j] = & Reset::BlackIsSafeFromCheckUpLeft;
      WhiteIsSafeFromRevealedCheckIndex[i][j] = LOOKUPLEFT;
      BlackIsSafeFromRevealedCheckIndex[i][j] = LOOKUPLEFT;
      WhiteIsSafeFromDirectCheckIndex[i][j] = LOOKUPLEFT;
      BlackIsSafeFromDirectCheckIndex[i][j] = LOOKUPLEFT;
    }
    bTemp = i_Bit;
    j = i;
    while(bTemp & UREDGE)
    {
      bTemp = (bTemp & UREDGE) >> 9;
      j += 9;
      WhiteIsSafeFromRevealedCheck[i][j] = & Reset::WhiteIsSafeFromRevealedCheckUpRight;
      BlackIsSafeFromRevealedCheck[i][j] = & Reset::BlackIsSafeFromRevealedCheckUpRight;
      WhiteIsSafeFromCheck[i][j] = & Reset::WhiteIsSafeFromCheckUpRight;
      BlackIsSafeFromCheck[i][j] = & Reset::BlackIsSafeFromCheckUpRight;
      WhiteIsSafeFromRevealedCheckIndex[i][j] = LOOKUPRIGHT;
      BlackIsSafeFromRevealedCheckIndex[i][j] = LOOKUPRIGHT;
      WhiteIsSafeFromDirectCheckIndex[i][j] = LOOKUPRIGHT;
      BlackIsSafeFromDirectCheckIndex[i][j] = LOOKUPRIGHT;
    }
    bTemp = i_Bit;
    j = i;
    while(bTemp & DLEDGE)
    {
      bTemp = (bTemp & DLEDGE) << 9;
      j -= 9;
      WhiteIsSafeFromRevealedCheck[i][j] = & Reset::WhiteIsSafeFromRevealedCheckDownLeft;
      BlackIsSafeFromRevealedCheck[i][j] = & Reset::BlackIsSafeFromRevealedCheckDownLeft;
      WhiteIsSafeFromCheck[i][j] = & Reset::WhiteIsSafeFromCheckDownLeft;
      BlackIsSafeFromCheck[i][j] = & Reset::BlackIsSafeFromCheckDownLeft;
      WhiteIsSafeFromRevealedCheckIndex[i][j] = LOOKDOWNLEFT;
      BlackIsSafeFromRevealedCheckIndex[i][j] = LOOKDOWNLEFT;
      WhiteIsSafeFromDirectCheckIndex[i][j] = LOOKDOWNLEFT;
      BlackIsSafeFromDirectCheckIndex[i][j] = LOOKDOWNLEFT;
    }
    bTemp = i_Bit;
    j = i;
    while(bTemp & DREDGE)
    {
      bTemp = (bTemp & DREDGE) << 7;
      j -= 7;
      WhiteIsSafeFromRevealedCheck[i][j] = & Reset::WhiteIsSafeFromRevealedCheckDownRight;
      BlackIsSafeFromRevealedCheck[i][j] = & Reset::BlackIsSafeFromRevealedCheckDownRight;
      WhiteIsSafeFromCheck[i][j] = & Reset::WhiteIsSafeFromCheckDownRight;
      BlackIsSafeFromCheck[i][j] = & Reset::BlackIsSafeFromCheckDownRight;
      WhiteIsSafeFromRevealedCheckIndex[i][j] = LOOKDOWNRIGHT;
      BlackIsSafeFromRevealedCheckIndex[i][j] = LOOKDOWNRIGHT;
      WhiteIsSafeFromDirectCheckIndex[i][j] = LOOKDOWNRIGHT;
      BlackIsSafeFromDirectCheckIndex[i][j] = LOOKDOWNRIGHT;
    }
  
    /* Rook or Queen */
    bTemp = i_Bit;
    j = i;
    while(bTemp & CANMOVEUP)
    {
      bTemp = (bTemp & CANMOVEUP) >> 8;
      j += 8;
      WhiteIsSafeFromRevealedCheck[i][j] = & Reset::WhiteIsSafeFromRevealedCheckUp;
      BlackIsSafeFromRevealedCheck[i][j] = & Reset::BlackIsSafeFromRevealedCheckUp;
      WhiteIsSafeFromCheck[i][j] = & Reset::WhiteIsSafeFromCheckUp;
      BlackIsSafeFromCheck[i][j] = & Reset::BlackIsSafeFromCheckUp;
      WhiteIsSafeFromRevealedCheckIndex[i][j] = LOOKUP;
      BlackIsSafeFromRevealedCheckIndex[i][j] = LOOKUP;
      WhiteIsSafeFromDirectCheckIndex[i][j] = LOOKUP;
      BlackIsSafeFromDirectCheckIndex[i][j] = LOOKUP;
    }
    bTemp = i_Bit;
    j = i;
    while(bTemp & CANMOVEDOWN)
    {
      bTemp = (bTemp & CANMOVEDOWN) << 8;
      j -= 8;
      WhiteIsSafeFromRevealedCheck[i][j] = & Reset::WhiteIsSafeFromRevealedCheckDown;
      BlackIsSafeFromRevealedCheck[i][j] = & Reset::BlackIsSafeFromRevealedCheckDown;
      WhiteIsSafeFromCheck[i][j] = & Reset::WhiteIsSafeFromCheckDown;
      BlackIsSafeFromCheck[i][j] = & Reset::BlackIsSafeFromCheckDown;
      WhiteIsSafeFromRevealedCheckIndex[i][j] = LOOKDOWN;
      BlackIsSafeFromRevealedCheckIndex[i][j] = LOOKDOWN;
      WhiteIsSafeFromDirectCheckIndex[i][j] = LOOKDOWN;
      BlackIsSafeFromDirectCheckIndex[i][j] = LOOKDOWN;
    }
    bTemp = i_Bit;
    j = i;
    while(bTemp & CANMOVERIGHT)
    {
      bTemp = (bTemp & CANMOVERIGHT) >> 1;
      j += 1;
      WhiteIsSafeFromRevealedCheck[i][j] = & Reset::WhiteIsSafeFromRevealedCheckRight;
      BlackIsSafeFromRevealedCheck[i][j] = & Reset::BlackIsSafeFromRevealedCheckRight;
      WhiteIsSafeFromCheck[i][j] = & Reset::WhiteIsSafeFromCheckRight;
      BlackIsSafeFromCheck[i][j] = & Reset::BlackIsSafeFromCheckRight;
      WhiteIsSafeFromRevealedCheckIndex[i][j] = LOOKRIGHT;
      BlackIsSafeFromRevealedCheckIndex[i][j] = LOOKRIGHT;
      WhiteIsSafeFromDirectCheckIndex[i][j] = LOOKRIGHT;
      BlackIsSafeFromDirectCheckIndex[i][j] = LOOKRIGHT;
    }
    bTemp = i_Bit;
    j = i;
    while(bTemp & CANMOVELEFT)
    {
      bTemp = (bTemp & CANMOVELEFT) << 1;
      j -= 1;
      WhiteIsSafeFromRevealedCheck[i][j] = & Reset::WhiteIsSafeFromRevealedCheckLeft;
      BlackIsSafeFromRevealedCheck[i][j] = & Reset::BlackIsSafeFromRevealedCheckLeft;
      WhiteIsSafeFromCheck[i][j] = & Reset::WhiteIsSafeFromCheckLeft;
      BlackIsSafeFromCheck[i][j] = & Reset::BlackIsSafeFromCheckLeft;
      WhiteIsSafeFromRevealedCheckIndex[i][j] = LOOKLEFT;
      BlackIsSafeFromRevealedCheckIndex[i][j] = LOOKLEFT;
      WhiteIsSafeFromDirectCheckIndex[i][j] = LOOKLEFT;
      BlackIsSafeFromDirectCheckIndex[i][j] = LOOKLEFT;
    }

    if ((i_Bit & K0100) >> 17)
    {
      WhiteIsSafeFromCheck[i][i+17] = & Reset::WhiteIsSafeFromCheckByKnight;
      BlackIsSafeFromCheck[i][i+17] = & Reset::BlackIsSafeFromCheckByKnight;
      WhiteIsSafeFromDirectCheckIndex[i][i+17] = LOOKKNIGHT;
      BlackIsSafeFromDirectCheckIndex[i][i+17] = LOOKKNIGHT;
    }
    if ((i_Bit & K0200) >> 10)
    {
      WhiteIsSafeFromCheck[i][i+10] = & Reset::WhiteIsSafeFromCheckByKnight;
      BlackIsSafeFromCheck[i][i+10] = & Reset::BlackIsSafeFromCheckByKnight;
      WhiteIsSafeFromDirectCheckIndex[i][i+10] = LOOKKNIGHT;
      BlackIsSafeFromDirectCheckIndex[i][i+10] = LOOKKNIGHT;
    }
    if ((i_Bit & K0400) << 6)
    {
      WhiteIsSafeFromCheck[i][i-6] = & Reset::WhiteIsSafeFromCheckByKnight;
      BlackIsSafeFromCheck[i][i-6] = & Reset::BlackIsSafeFromCheckByKnight;
      WhiteIsSafeFromDirectCheckIndex[i][i-6] = LOOKKNIGHT;
      BlackIsSafeFromDirectCheckIndex[i][i-6] = LOOKKNIGHT;
    }
    if ((i_Bit & K0500) << 15)
    {
      WhiteIsSafeFromCheck[i][i-15] = & Reset::WhiteIsSafeFromCheckByKnight;
      BlackIsSafeFromCheck[i][i-15] = & Reset::BlackIsSafeFromCheckByKnight;
      WhiteIsSafeFromDirectCheckIndex[i][i-15] = LOOKKNIGHT;
      BlackIsSafeFromDirectCheckIndex[i][i-15] = LOOKKNIGHT;
    }
    if ((i_Bit & K0700) << 17)
    {
      WhiteIsSafeFromCheck[i][i-17] = & Reset::WhiteIsSafeFromCheckByKnight;
      BlackIsSafeFromCheck[i][i-17] = & Reset::BlackIsSafeFromCheckByKnight;
      WhiteIsSafeFromDirectCheckIndex[i][i-17] = LOOKKNIGHT;
      BlackIsSafeFromDirectCheckIndex[i][i-17] = LOOKKNIGHT;
    }
    if ((i_Bit & K0800) << 10)
    {
      WhiteIsSafeFromCheck[i][i-10] = & Reset::WhiteIsSafeFromCheckByKnight;
      BlackIsSafeFromCheck[i][i-10] = & Reset::BlackIsSafeFromCheckByKnight;
      WhiteIsSafeFromDirectCheckIndex[i][i-10] = LOOKKNIGHT;
      BlackIsSafeFromDirectCheckIndex[i][i-10] = LOOKKNIGHT;
    }
    if ((i_Bit & K1000) >> 6)
    {
      WhiteIsSafeFromCheck[i][i+6] = & Reset::WhiteIsSafeFromCheckByKnight;
      BlackIsSafeFromCheck[i][i+6] = & Reset::BlackIsSafeFromCheckByKnight;
      WhiteIsSafeFromDirectCheckIndex[i][i+6] = LOOKKNIGHT;
      BlackIsSafeFromDirectCheckIndex[i][i+6] = LOOKKNIGHT;
    }
    if ((i_Bit & K1100) >> 15)
    {
      WhiteIsSafeFromCheck[i][i+15] = & Reset::WhiteIsSafeFromCheckByKnight;
      BlackIsSafeFromCheck[i][i+15] = & Reset::BlackIsSafeFromCheckByKnight;
      WhiteIsSafeFromDirectCheckIndex[i][i+15] = LOOKKNIGHT;
      BlackIsSafeFromDirectCheckIndex[i][i+15] = LOOKKNIGHT;
    }
  }
}


void InitKingStars()
{
  int i,j;
  unsigned long long int BitMask = 0x8000000000000000;
  unsigned long long int j_Bit, Temp;
  
  for (j=0;j<64;j++)
  {
    j_Bit = BitMask >> j;
    KingStars[j] = 0x0 | j_Bit;
    
    /* Bishop or Queen */
    Temp = j_Bit;
    while(Temp & ULEDGE)
    {
      Temp = (Temp & ULEDGE) >> 7;
      KingStars[j] |= Temp;
    }
    Temp = j_Bit;
    while(Temp & UREDGE)
    {
      Temp = (Temp & UREDGE) >> 9;
      KingStars[j] |= Temp;
    }
    Temp = j_Bit;
    while(Temp & DLEDGE)
    {
      Temp = (Temp & DLEDGE) << 9;
      KingStars[j] |= Temp;
    }
    Temp = j_Bit;
    while(Temp & DREDGE)
    {
      Temp = (Temp & DREDGE) << 7;
      KingStars[j] |= Temp;
    }
  
    /* Rook or Queen */
    Temp = j_Bit;
    while(Temp & CANMOVEUP)
    {
      Temp = (Temp & CANMOVEUP) >> 8;
      KingStars[j] |= Temp;
    }
    Temp = j_Bit;
    while(Temp & CANMOVEDOWN)
    {
      Temp = (Temp & CANMOVEDOWN) << 8;
      KingStars[j] |= Temp;
    }
    Temp = j_Bit;
    while(Temp & CANMOVERIGHT)
    {
      Temp = (Temp & CANMOVERIGHT) >> 1;
      KingStars[j] |= Temp;
    }
    Temp = j_Bit;
    while(Temp & CANMOVELEFT)
    {
      Temp = (Temp & CANMOVELEFT) << 1;
      KingStars[j] |= Temp;
    }
  }
}


void InitAllAttacks()
{
  int i,j;
  unsigned long long int BitMask = 0x8000000000000000;
  unsigned long long int j_Bit, Temp;
  
  for (j=0;j<64;j++)
  {
    AllAttacks[j] = KingStars[j];

    j_Bit = BitMask >> j;

    AllAttacks[j] |= ((j_Bit & K0100) >> 17);
    AllAttacks[j] |= ((j_Bit & K0200) >> 10);
    AllAttacks[j] |= ((j_Bit & K0400) << 6);
    AllAttacks[j] |= ((j_Bit & K0500) << 15);
    AllAttacks[j] |= ((j_Bit & K0700) << 17);
    AllAttacks[j] |= ((j_Bit & K0800) << 10);
    AllAttacks[j] |= ((j_Bit & K1000) >> 6);
    AllAttacks[j] |= ((j_Bit & K1100) >> 15);
  }
}


int Reset::SafeFromCheck()
{
  return TRUE;
}


int Reset::WhiteIsSafeFromRevealedCheckUp()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Rook or Queen */
  Temp = bWhite & bKings;
  if (Attackers = bBlack & (bRooks | bQueens))
  {
    while(Temp & CANMOVEUP)
    {
      Temp = (Temp & CANMOVEUP) >> 8;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromRevealedCheckUpRight()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Bishop or Queen */
  Temp = bWhite & bKings;
  if (Attackers = bBlack & (bBishops | bQueens))
  {
    while(Temp & UREDGE)
    {
      Temp = (Temp & UREDGE) >> 9;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromRevealedCheckRight()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Rook or Queen */
  Temp = bWhite & bKings;
  if (Attackers = bBlack & (bRooks | bQueens))
  {
    while(Temp & CANMOVERIGHT)
    {
      Temp = (Temp & CANMOVERIGHT) >> 1;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromRevealedCheckDownRight()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Bishop or Queen */
  Temp = bWhite & bKings;
  if (Attackers = bBlack & (bBishops | bQueens))
  {
    while(Temp & DREDGE)
    {
      Temp = (Temp & DREDGE) << 7;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromRevealedCheckDown()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Rook or Queen */
  Temp = bWhite & bKings;
  if (Attackers = bBlack & (bRooks | bQueens))
  {
    while(Temp & CANMOVEDOWN)
    {
      Temp = (Temp & CANMOVEDOWN) << 8;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromRevealedCheckDownLeft()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Bishop or Queen */
  Temp = bWhite & bKings;
  if (Attackers = bBlack & (bBishops | bQueens))
  {
    while(Temp & DLEDGE)
    {
      Temp = (Temp & DLEDGE) << 9;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromRevealedCheckLeft()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Rook or Queen */
  Temp = bWhite & bKings;
  if (Attackers = bBlack & (bRooks | bQueens))
  {
    while(Temp & CANMOVELEFT)
    {
      Temp = (Temp & CANMOVELEFT) << 1;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromRevealedCheckUpLeft()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Bishop or Queen */
  Temp = bWhite & bKings;
  if (Attackers = bBlack & (bBishops | bQueens))
  {
    while(Temp & ULEDGE)
    {
      Temp = (Temp & ULEDGE) >> 7;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::BlackIsSafeFromRevealedCheckUp()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Rook or Queen */
  Temp = bBlack & bKings;
  if (Attackers = bWhite & (bRooks | bQueens))
  {
    while(Temp & CANMOVEUP)
    {
      Temp = (Temp & CANMOVEUP) >> 8;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::BlackIsSafeFromRevealedCheckUpRight()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Bishop or Queen */
  Temp = bBlack & bKings;
  if (Attackers = bWhite & (bBishops | bQueens))
  {
    while(Temp & UREDGE)
    {
      Temp = (Temp & UREDGE) >> 9;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::BlackIsSafeFromRevealedCheckRight()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Rook or Queen */
  Temp = bBlack & bKings;
  if (Attackers = bWhite & (bRooks | bQueens))
  {
    while(Temp & CANMOVERIGHT)
    {
      Temp = (Temp & CANMOVERIGHT) >> 1;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::BlackIsSafeFromRevealedCheckDownRight()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Bishop or Queen */
  Temp = bBlack & bKings;
  if (Attackers = bWhite & (bBishops | bQueens))
  {
    while(Temp & DREDGE)
    {
      Temp = (Temp & DREDGE) << 7;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::BlackIsSafeFromRevealedCheckDown()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Rook or Queen */
  Temp = bBlack & bKings;
  if (Attackers = bWhite & (bRooks | bQueens))
  {
    while(Temp & CANMOVEDOWN)
    {
      Temp = (Temp & CANMOVEDOWN) << 8;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::BlackIsSafeFromRevealedCheckDownLeft()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Bishop or Queen */
  Temp = bBlack & bKings;
  if (Attackers = bWhite & (bBishops | bQueens))
  {
    while(Temp & DLEDGE)
    {
      Temp = (Temp & DLEDGE) << 9;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::BlackIsSafeFromRevealedCheckLeft()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Rook or Queen */
  Temp = bBlack & bKings;
  if (Attackers = bWhite & (bRooks | bQueens))
  {
    while(Temp & CANMOVELEFT)
    {
      Temp = (Temp & CANMOVELEFT) << 1;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::BlackIsSafeFromRevealedCheckUpLeft()
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Bishop or Queen */
  Temp = bBlack & bKings;
  if (Attackers = bWhite & (bBishops | bQueens))
  {
    while(Temp & ULEDGE)
    {
      Temp = (Temp & ULEDGE) >> 7;
      if (Temp & Attackers)
        return FALSE;
      Temp &= ~bAll;
    }
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromCheckUp()
{
  if (bTo & (bRooks | bQueens))
  {
    return (WhiteIsSafeFromRevealedCheckUp());
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromCheckUpRight()
{
  if (bTo & (bPawns | bQueens | bBishops))
  {
    /* Pawn */
    if (((bWhite & bKings & UREDGE) >> 9) & bTo & bPawns)
      return FALSE;
  
    /* Bishop or Queen */
    if (bTo & (bBishops | bQueens))
      return (WhiteIsSafeFromRevealedCheckUpRight());
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromCheckRight()
{
  if (bTo & (bRooks | bQueens))
  {
    return (WhiteIsSafeFromRevealedCheckRight());
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromCheckDownRight()
{
  if (bTo & (bBishops | bQueens))
  {
    return (WhiteIsSafeFromRevealedCheckDownRight());
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromCheckDown()
{
  if (bTo & (bRooks | bQueens))
  {
    return (WhiteIsSafeFromRevealedCheckDown());
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromCheckDownLeft()
{
  if (bTo & (bBishops | bQueens))
  {
    return (WhiteIsSafeFromRevealedCheckDownLeft());
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromCheckLeft()
{
  if (bTo & (bRooks | bQueens))
  {
    return (WhiteIsSafeFromRevealedCheckLeft());
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromCheckUpLeft()
{
  if (bTo & (bPawns | bBishops | bQueens))
  {
    /* Pawn */
    if (((bWhite & bKings & ULEDGE) >> 7) & bTo & bPawns)
      return FALSE;

    /* Bishop or Queen */
    if (bTo & (bBishops | bQueens))
      return (WhiteIsSafeFromRevealedCheckUpLeft());
  }
  return TRUE;
}


int Reset::BlackIsSafeFromCheckUp()
{
  if (bTo & (bRooks | bQueens))
  {
    return (BlackIsSafeFromRevealedCheckUp());
  }
  return TRUE;
}


int Reset::BlackIsSafeFromCheckUpRight()
{
  if (bTo & (bBishops | bQueens))
  {
    return (BlackIsSafeFromRevealedCheckUpRight());
  }
  return TRUE;
}


int Reset::BlackIsSafeFromCheckRight()
{
  if (bTo & (bRooks | bQueens))
  {
    return (BlackIsSafeFromRevealedCheckRight());
  }
  return TRUE;
}


int Reset::BlackIsSafeFromCheckDownRight()
{
  if (bTo & (bPawns | bBishops | bQueens))
  {
    /* Pawns */
    if (((bBlack & bKings & DREDGE) << 7) & bTo & bPawns)
      return FALSE;
  
    /* Bishop or Queen */
    if (bTo & (bBishops | bQueens))
      return (BlackIsSafeFromRevealedCheckDownRight());
  }
  return TRUE;
}


int Reset::BlackIsSafeFromCheckDown()
{
  if (bTo & (bRooks | bQueens))
  {
    return (BlackIsSafeFromRevealedCheckDown());
  }
  return TRUE;
}


int Reset::BlackIsSafeFromCheckDownLeft()
{
  if (bTo & (bPawns | bBishops | bQueens))
  {
    /* Pawns */
    if (((bBlack & bKings & DLEDGE) << 9) & bTo & bPawns)
      return FALSE;
  
    /* Bishop or Queen */
    if (bTo & (bBishops | bQueens))
      return (BlackIsSafeFromRevealedCheckDownLeft());
  }
  return TRUE;
}


int Reset::BlackIsSafeFromCheckLeft()
{
  if (bTo & (bRooks | bQueens))
  {
    return (BlackIsSafeFromRevealedCheckLeft());
  }
  return TRUE;
}


int Reset::BlackIsSafeFromCheckUpLeft()
{
  if (bTo & (bBishops | bQueens))
  {
    return (BlackIsSafeFromRevealedCheckUpLeft());
  }
  return TRUE;
}


int Reset::WhiteIsSafeFromCheckByKnight()
{
  //Assuming the routing procedure knew To was a knight's move from White king
  if (bTo & bKnights)
    return FALSE;
  else
    return TRUE;
}


int Reset::BlackIsSafeFromCheckByKnight()
{
  //Assuming the routing procedure knew To was a knight's move from Black king
  if (bTo & bKnights)
    return FALSE;
  else
    return TRUE;
}


//
//PERFORMANCE PATH
//
int Reset::WhiteIsSafe(unsigned long long int Squares)
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Pawns or Diagonal Kings */
  if (((Squares & ULEDGE) >> 7) & bBlack & (bPawns | bKings))
    return FALSE;
  if (((Squares & UREDGE) >> 9) & bBlack & (bPawns | bKings))
    return FALSE;
  //Note Assumes En Passant doesnt apply here

  /* Bishop or Queen */
  Temp = Squares;
  Attackers = bBlack & (bBishops | bQueens);
  while(Temp & ULEDGE)
  {
    Temp = (Temp & ULEDGE) >> 7;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  Temp = Squares;
  while(Temp & UREDGE)
  {
    Temp = (Temp & UREDGE) >> 9;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  Temp = Squares;
  while(Temp & DLEDGE)
  {
    Temp = (Temp & DLEDGE) << 9;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  Temp = Squares;
  while(Temp & DREDGE)
  {
    Temp = (Temp & DREDGE) << 7;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }

  /* Rook or Queen */
  Temp = Squares;
  Attackers = bBlack & (bRooks | bQueens);
  while(Temp & CANMOVEUP)
  {
    Temp = (Temp & CANMOVEUP) >> 8;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  Temp = Squares;
  while(Temp & CANMOVEDOWN)
  {
    Temp = (Temp & CANMOVEDOWN) << 8;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  Temp = Squares;
  while(Temp & CANMOVERIGHT)
  {
    Temp = (Temp & CANMOVERIGHT) >> 1;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  Temp = Squares;
  while(Temp & CANMOVELEFT)
  {
    Temp = (Temp & CANMOVELEFT) << 1;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  
  /* Knights */
  Attackers = bKnights & bBlack;
  if (Attackers & ((Squares & K0100) >> 17))
    return FALSE;
  if (Attackers & ((Squares & K0200) >> 10))
    return FALSE;
  if (Attackers & ((Squares & K0400) << 6))
    return FALSE;
  if (Attackers & ((Squares & K0500) << 15))
    return FALSE;
  if (Attackers & ((Squares & K0700) << 17))
    return FALSE;
  if (Attackers & ((Squares & K0800) << 10))
    return FALSE;
  if (Attackers & ((Squares & K1000) >> 6))
    return FALSE;
  if (Attackers & ((Squares & K1100) >> 15))
    return FALSE;

  /* King */
  Attackers = bKings & bBlack;
  if (((Squares & CANMOVEUP) >> 8) & Attackers)
    return FALSE;
  if (((Squares & CANMOVERIGHT) >> 1) & Attackers)
    return FALSE;
  if (((Squares & CANMOVELEFT) << 1) & Attackers)
    return FALSE;
  if (((Squares & CANMOVEDOWN) << 8) & Attackers)
    return FALSE;
  if (((Squares & DREDGE) << 7) & Attackers)
    return FALSE;
  if (((Squares & DLEDGE) << 9) & Attackers)
    return FALSE;

  return TRUE;
}




//
//PERFORMANCE PATH
//
int Reset::BlackIsSafe(unsigned long long int Squares)
{
  unsigned long long int Temp;
  unsigned long long int Attackers;

  /* Pawns or Diagonal Kings */
  if (((Squares & DLEDGE) << 9) & bWhite & (bPawns | bKings))
    return FALSE;
  if (((Squares & DREDGE) << 7) & bWhite & (bPawns | bKings))
    return FALSE;
  //Note Assumes En Passant doesnt apply here

  /* Bishop or Queen */
  Temp = Squares;
  Attackers = bWhite & (bBishops | bQueens);
  while(Temp & ULEDGE)
  {
    Temp = (Temp & ULEDGE) >> 7;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  Temp = Squares;
  while(Temp & UREDGE)
  {
    Temp = (Temp & UREDGE) >> 9;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  Temp = Squares;
  while(Temp & DLEDGE)
  {
    Temp = (Temp & DLEDGE) << 9;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  Temp = Squares;
  while(Temp & DREDGE)
  {
    Temp = (Temp & DREDGE) << 7;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }

  /* Rook or Queen */
  Temp = Squares;
  Attackers = bWhite & (bRooks | bQueens);
  while(Temp & CANMOVEUP)
  {
    Temp = (Temp & CANMOVEUP) >> 8;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  Temp = Squares;
  while(Temp & CANMOVEDOWN)
  {
    Temp = (Temp & CANMOVEDOWN) << 8;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  Temp = Squares;
  while(Temp & CANMOVERIGHT)
  {
    Temp = (Temp & CANMOVERIGHT) >> 1;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  Temp = Squares;
  while(Temp & CANMOVELEFT)
  {
    Temp = (Temp & CANMOVELEFT) << 1;
    if (Temp & Attackers)
      return FALSE;
    Temp &= ~bAll;
  }
  
  /* Knights */
  Attackers = bKnights & bWhite;
  if (Attackers & ((Squares & K0100) >> 17))
    return FALSE;
  if (Attackers & ((Squares & K0200) >> 10))
    return FALSE;
  if (Attackers & ((Squares & K0400) << 6))
    return FALSE;
  if (Attackers & ((Squares & K0500) << 15))
    return FALSE;
  if (Attackers & ((Squares & K0700) << 17))
    return FALSE;
  if (Attackers & ((Squares & K0800) << 10))
    return FALSE;
  if (Attackers & ((Squares & K1000) >> 6))
    return FALSE;
  if (Attackers & ((Squares & K1100) >> 15))
    return FALSE;

  /* King */
  Attackers = bKings & bWhite;
  if (((Squares & CANMOVEUP) >> 8) & Attackers)
    return FALSE;
  if (((Squares & CANMOVERIGHT) >> 1) & Attackers)
    return FALSE;
  if (((Squares & CANMOVELEFT) << 1) & Attackers)
    return FALSE;
  if (((Squares & CANMOVEDOWN) << 8) & Attackers)
    return FALSE;
  if (((Squares & UREDGE) >> 9) & Attackers)
    return FALSE;
  if (((Squares & ULEDGE) >> 7) & Attackers)
    return FALSE;

  return TRUE;
}







void Reset::LookForWhiteKingInCheck()
{
  if (
      (
       (ToMove == WHITE) && 				//Black just moved
       (
        !(
          (bTo & AllAttacks[WhiteKingSquare]) || 
          (bFrom & KingStars[WhiteKingSquare])
          )
       ) &&
       (!MustCheckSafety) 
      ) ||
      (
       (ToMove == BLACK) && 				//White just moved
       (
        !(bFrom & KingStars[WhiteKingSquare])
       ) &&
       (!MustCheckSafety)
      )
     )
  {
      WhiteInCheck = OFF;
  }
  else
  {
    if (WhiteIsSafe(bWhite & bKings))
      WhiteInCheck = OFF;
    else
      WhiteInCheck = ON;
  }
}



void Reset::LookForBlackKingInCheck()
{
  if (
      (
       (ToMove == BLACK) && 				//White just moved
       (
        !(
          (bTo & AllAttacks[BlackKingSquare]) || 
          (bFrom & KingStars[BlackKingSquare])
          )
       ) &&
       (!MustCheckSafety)
      ) ||
      (
       (ToMove == WHITE) && 				//Black just moved
       (
        !(bFrom & KingStars[BlackKingSquare])
       ) &&
       (!MustCheckSafety)
      )
     )
  {
    BlackInCheck = OFF;
  }
  else
  {
    if (BlackIsSafe(bBlack & bKings))
      BlackInCheck = OFF;
    else
      BlackInCheck = ON;
  }
}



//
//Performance Path
//
//Note: Order these in the most likely directions
int Reset::WhiteRevealedCheckRouter(signed char Square)
{
  switch(WhiteIsSafeFromRevealedCheckIndex[WhiteKingSquare][Square])
  {
    case SAFE:
      return TRUE;
    case LOOKUP:
      return WhiteIsSafeFromRevealedCheckUp();
    case LOOKUPRIGHT:
      return WhiteIsSafeFromRevealedCheckUpRight();
    case LOOKRIGHT:
      return WhiteIsSafeFromRevealedCheckRight();
    case LOOKDOWNRIGHT:
      return WhiteIsSafeFromRevealedCheckDownRight();
    case LOOKDOWN:
      return WhiteIsSafeFromRevealedCheckDown();
    case LOOKDOWNLEFT:
      return WhiteIsSafeFromRevealedCheckDownLeft();
    case LOOKLEFT:
      return WhiteIsSafeFromRevealedCheckLeft();
    default: //UPLEFT
      return WhiteIsSafeFromRevealedCheckUpLeft();
  }
}



//
//Performance Path
//
//Note: Order these in the most likely directions
int Reset::BlackRevealedCheckRouter(signed char Square)
{
  switch(BlackIsSafeFromRevealedCheckIndex[BlackKingSquare][Square])
  {
    case SAFE:
      return TRUE;
    case LOOKUP:
      return BlackIsSafeFromRevealedCheckUp();
    case LOOKUPRIGHT:
      return BlackIsSafeFromRevealedCheckUpRight();
    case LOOKRIGHT:
      return BlackIsSafeFromRevealedCheckRight();
    case LOOKDOWNRIGHT:
      return BlackIsSafeFromRevealedCheckDownRight();
    case LOOKDOWN:
      return BlackIsSafeFromRevealedCheckDown();
    case LOOKDOWNLEFT:
      return BlackIsSafeFromRevealedCheckDownLeft();
    case LOOKLEFT:
      return BlackIsSafeFromRevealedCheckLeft();
    default: //UPLEFT
      return BlackIsSafeFromRevealedCheckUpLeft();
  }
}



//
//Performance Path
//
//Note: Order these in the most likely directions
int Reset::WhiteDirectCheckRouter(signed char Square)
{
  switch(WhiteIsSafeFromDirectCheckIndex[WhiteKingSquare][Square])
  {
    case SAFE:
      return TRUE;
    case LOOKUP:
      return WhiteIsSafeFromCheckUp();
    case LOOKUPRIGHT:
      return WhiteIsSafeFromCheckUpRight();
    case LOOKRIGHT:
      return WhiteIsSafeFromCheckRight();
    case LOOKDOWNRIGHT:
      return WhiteIsSafeFromCheckDownRight();
    case LOOKDOWN:
      return WhiteIsSafeFromCheckDown();
    case LOOKDOWNLEFT:
      return WhiteIsSafeFromCheckDownLeft();
    case LOOKLEFT:
      return WhiteIsSafeFromCheckLeft();
    case LOOKUPLEFT:
      return WhiteIsSafeFromCheckUpLeft();
    default: //KNIGHT
      return WhiteIsSafeFromCheckByKnight();
  }
}



//
//Performance Path
//
//Note: Order these in the most likely directions
int Reset::BlackDirectCheckRouter(signed char Square)
{
  switch(BlackIsSafeFromDirectCheckIndex[BlackKingSquare][Square])
  {
    case SAFE:
      return TRUE;
    case LOOKUP:
      return BlackIsSafeFromCheckUp();
    case LOOKUPRIGHT:
      return BlackIsSafeFromCheckUpRight();
    case LOOKRIGHT:
      return BlackIsSafeFromCheckRight();
    case LOOKDOWNRIGHT:
      return BlackIsSafeFromCheckDownRight();
    case LOOKDOWN:
      return BlackIsSafeFromCheckDown();
    case LOOKDOWNLEFT:
      return BlackIsSafeFromCheckDownLeft();
    case LOOKLEFT:
      return BlackIsSafeFromCheckLeft();
    case LOOKUPLEFT:
      return BlackIsSafeFromCheckUpLeft();
    default: //KNIGHT
      return BlackIsSafeFromCheckByKnight();
  }
}



int Reset::DidWhiteJustMoveIntoCheck()
{
#ifdef OLDWAY
  if (
      (
       !(bFrom & KingStars[WhiteKingSquare])
      ) &&
      (!MustCheckSafety)
     )
  {
    return(FALSE);
  }
  else
  {
    if (WhiteIsSafe(bWhite & bKings))
      return(FALSE);
    else
      return(TRUE);
  }
#endif
#ifdef NEWWAY
  if (MustCheckSafety)
  {
    if (WhiteIsSafe(bWhite & bKings))
      return(FALSE);
    else
      return(TRUE);
  }
  else
  {
    if (CALL_MEMBER_FUNCTION(*this,WhiteIsSafeFromRevealedCheck[WhiteKingSquare][From])())
      return(FALSE);
    else
      return(TRUE);
  }
#endif
#ifdef NEWERWAY
  if (MustCheckSafety)
  {
    if (WhiteIsSafe(bWhite & bKings))
      return(FALSE);
    else
      return(TRUE);
  }
  else
  {
    if (WhiteRevealedCheckRouter(From))
      return(FALSE);
    else
      return(TRUE);
  }
#endif
}




int Reset::DidBlackJustMoveIntoCheck()
{
#ifdef OLDWAY
  if (
      (
       !(bFrom & KingStars[BlackKingSquare])
      ) &&
      (!MustCheckSafety)
     )
  {
    return(FALSE);
  }
  else
  {
    if (BlackIsSafe(bBlack & bKings))
      return(FALSE);
    else
      return(TRUE);
  }
#endif
#ifdef NEWWAY
  if (MustCheckSafety)
  {
    if (BlackIsSafe(bBlack & bKings))
      return(FALSE);
    else
      return(TRUE);
  }
  else
  {
    if (CALL_MEMBER_FUNCTION(*this,BlackIsSafeFromRevealedCheck[BlackKingSquare][From])())
      return(FALSE);
    else
      return(TRUE);
  }
#endif
#ifdef NEWERWAY
  if (MustCheckSafety)
  {
    if (BlackIsSafe(bBlack & bKings))
      return(FALSE);
    else
      return(TRUE);
  }
  else
  {
    if (BlackRevealedCheckRouter(From))
      return(FALSE);
    else
      return(TRUE);
  }
#endif
}




int Reset::DidWhiteMoveCauseBlackCheck()
{
#ifdef OLDWAY
  if (
      (
       !(
         (bTo & AllAttacks[BlackKingSquare]) || 
         (bFrom & KingStars[BlackKingSquare])
        )
      ) &&
      (!MustCheckSafety)
     )
  {
    return(FALSE);
  }
  else
  {
    if (BlackIsSafe(bBlack & bKings))
      return(FALSE);
    else
      return(TRUE);
  }
#endif
#ifdef NEWWAY
  if (MustCheckSafety)
  {
    if (BlackIsSafe(bBlack & bKings))
      return(FALSE);
    else
      return(TRUE);
  }
  else
  {
    if ((CALL_MEMBER_FUNCTION(*this,BlackIsSafeFromRevealedCheck[BlackKingSquare][From])()) &&
        (CALL_MEMBER_FUNCTION(*this,BlackIsSafeFromCheck[BlackKingSquare][To])()))
      return(FALSE);
    else
      return(TRUE);
  }
#endif
#ifdef NEWERWAY
  if (MustCheckSafety)
  {
    if (BlackIsSafe(bBlack & bKings))
      return(FALSE);
    else
      return(TRUE);
  }
  else
  {
    if ((BlackRevealedCheckRouter(From)) && (BlackDirectCheckRouter(To)))
      return(FALSE);
    else
      return(TRUE);
  }
#endif
}




int Reset::DidBlackMoveCauseWhiteCheck()
{
#ifdef OLDWAY
  if (
      (
       !(
         (bTo & AllAttacks[WhiteKingSquare]) || 
         (bFrom & KingStars[WhiteKingSquare])
        )
       ) &&
      (!MustCheckSafety)
     )
  {
    return(FALSE);
  }
  else
  {
    if (WhiteIsSafe(bWhite & bKings))
      return(FALSE);
    else
      return(TRUE);
  }
#endif
#ifdef NEWWAY
  if (MustCheckSafety)
  {
    if (WhiteIsSafe(bWhite & bKings))
      return(FALSE);
    else
      return(TRUE);
  }
  else
  {
    if ((CALL_MEMBER_FUNCTION(*this,WhiteIsSafeFromRevealedCheck[WhiteKingSquare][From])()) &&
        (CALL_MEMBER_FUNCTION(*this,WhiteIsSafeFromCheck[WhiteKingSquare][To])()))
      return(FALSE);
    else
      return(TRUE);
  }
#endif
#ifdef NEWERWAY
  if (MustCheckSafety)
  {
    if (WhiteIsSafe(bWhite & bKings))
      return(FALSE);
    else
      return(TRUE);
  }
  else
  {
    if ((WhiteRevealedCheckRouter(From)) && (WhiteDirectCheckRouter(To)))
      return(FALSE);
    else
      return(TRUE);
  }
#endif
}




int Reset::ValidChild(Reset *MyChild)
{
  Reset TempReset;

  //Set Check indicators 
  //  Either check achieved or player illegally moved into check

  if (ToMove == WHITE)
  {
    //If the move by white is invalid because the child has white in check
    //If this move is invalid because a player moved into check
    MyChild->LookForWhiteKingInCheck();
    if (MyChild->WhiteInCheck)
    {
      InitMyChild(MyChild);
      return(FALSE);
    }
    else
    {
      MyChild->LookForBlackKingInCheck();
    }
  }
  else //(ToMove == BLACK)
  {
    //If the move by black is invalid because the child has black in check
    MyChild->LookForBlackKingInCheck();
    if (MyChild->BlackInCheck)
    {
      InitMyChild(MyChild);
      return(FALSE);
    }
    else
    {
      MyChild->LookForWhiteKingInCheck();
    }
  }
  return(TRUE);
}
