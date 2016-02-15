#include "chess.h"

int HashMultiples[64];



#ifdef LOGIC

/* Returns HAVE_NOT_SEEN_MOVE if we haven't */
float HaveWeSeenMove(Reset *CurrentReset, int Depth)
{
  Reset *ptr;

  CurrentReset->ScoreDepth = Depth;
  ptr = HashTable[CurrentReset->HashValue % HASH_TABLE_SIZE];

  while (ptr != NULLRESET)
  {
    if (ptr->HashValue < CurrentReset->HashValue) {
      ptr = ptr->Sibling;
    } else if (ptr->HashValue > CurrentReset->HashValue) {
      return HAVE_NOT_SEEN_MOVE;
    } else {
      if ((ptr->ScoreDepth >= CurrentReset->ScoreDepth) &&
          (DoBoardsMatch(CurrentReset,ptr))) {
        if (ptr->HashCount != MAX_HASHCOUNT)
          (ptr->HashCount)++;
        return ptr->Score;
      } else {
        ptr = ptr->Sibling;
      }
    }
  }
  return HAVE_NOT_SEEN_MOVE;
}



void ReduceHashEntries()
{
  int i;
  Reset **ptr;
  Reset **EntryToDelete;
  Reset *tempptr;

  while (HashEntryCount > ReducedHashEntryCount)
  {
    /* Remove one Reset from each hash entry (if possible) */
    for (i=0;i<HASH_TABLE_SIZE;i++)
    {
      ptr = &(HashTable[i]);
      EntryToDelete = ptr;

      /* If the list for this hash value isn't empty */
      if (*EntryToDelete != NULLRESET)
      {
        ptr = &((*ptr)->Sibling);

        /* Run through the entry list once to see what's worth deleting */
        while (*ptr != NULLRESET)
        {
          if ((*ptr)->HashCount < (*EntryToDelete)->HashCount)
            EntryToDelete = ptr;
          else
          {
            if (((*ptr)->HashCount == (*EntryToDelete)->HashCount) &
                ((*ptr)->ScoreDepth < (*EntryToDelete)->ScoreDepth))
              EntryToDelete = ptr;
          }

          ptr = &((*ptr)->Sibling);
        }

        /* Delete the entry */
        tempptr = *EntryToDelete;
        *EntryToDelete = (*EntryToDelete)->Sibling;
        FreeReset(tempptr);
        --HashEntryCount;
      }
    }
  }
  /*printf("TO %d\n",HashEntryCount);*/
}





int OutDatedReset (Reset *HashReset, Reset *CurrentReset)
{
  int i;

  if (HashReset->NumPieces > CurrentReset->NumPieces)
    return (TRUE);
  if (HashReset->NumWhitePieces > CurrentReset->NumWhitePieces)
    return (TRUE);

  /* 
   * I can look to see if pawns have advanced here, but it looks pretty
   * difficult now that I'm "condensing" the Pieces and PieceType arrays.
   */

  return (FALSE);
}





void EliminateOldHashEntries(Reset *CurrentReset)
{
  int i;
  Reset **NextReset;
  Reset **EntryToDelete;
  Reset *tempptr;
  long int seen, deleted;

  /* At this point, since OutDatedReset only looks at captures,
   * I only need to go through this if I just captured */
  if (CurrentReset->Capture == ON) {
    seen = 0;
    deleted = 0;

    /* Remove one Reset from each hash entry (if possible) */
    for (i=0;i<HASH_TABLE_SIZE;i++)
    {
      EntryToDelete = &(HashTable[i]);
  
      /* Run through the entry list once to see what's worth deleting */
      while (*EntryToDelete != NULLRESET)
      {
        seen++;
        NextReset = &((*EntryToDelete)->Sibling);

        if (OutDatedReset(*EntryToDelete,CurrentReset))
        {
          /* Delete the entry */
          tempptr = *EntryToDelete;
          *EntryToDelete = (*NextReset);
          FreeReset(tempptr);
          --HashEntryCount;
          deleted++;
        } else {
          EntryToDelete = NextReset;
        }
      }
    }
#ifdef DEBUG_MODE
    fprintf(logfile,
            "EliminateOldHashEntries - %d seen, %d deleted\n",seen,deleted);
#endif
  }
}






/* Stores move in Hash Table */
void SaveMove(Reset *CurrentReset)
{
  Reset **ptr;
  Reset *MyCopy;

  MyCopy = GetReset();
  DuplicateReset(MyCopy,CurrentReset);
  MyCopy->HashCount = 0;

  ptr = &(HashTable[CurrentReset->HashValue % HASH_TABLE_SIZE]);
  while ((*ptr != NULLRESET) && ((*ptr)->HashValue < CurrentReset->HashValue))
    ptr = &((*ptr)->Sibling);

  /* Insert this reset BEFORE the reset *ptr is pointing to */
  MyCopy->Sibling = *ptr;
  *ptr = MyCopy;

  HashEntryCount++;
  if (HashEntryCount > MaxHashEntryCount)
    ReduceHashEntries();
}






void PrintHashTableInfo()
{
  int i;
  Reset *ptr;
  int Counts[50];
  int check = 0;
  int entrycount;

  for (i=0;i<50;i++)
    Counts[i] = 0;

  for (i=0;i<HASH_TABLE_SIZE;i++)
  {
    entrycount = 0;
    ptr = HashTable[i];
    while (ptr != NULLRESET)
    {
      entrycount++;
      check++;
      if (ptr->HashCount < 50)
        ++(Counts[ptr->HashCount]);
      else
        fprintf(logfile,"ABOVE 50!!\n");
      ptr = ptr->Sibling;
    }
    //if (entrycount > 0)
      //printf("Hash Value %d: %d entries\n",i,entrycount);
  }

/*  for (i=0;i<50;i++)
    printf("Counts[%d] = %d\n",i,Counts[i]); */

  fprintf(logfile,"Hash Table contains %d Entries (%d bytes)\n",
          HashEntryCount,HashEntryCount*sizeof(Reset));
}




void LowestScoreDepthInHashTable(Reset *LowestReset)
{
  int i;
  Reset *ptr;
  Reset *LowestSoFar = NULLRESET;

  for (i=0;i<HASH_TABLE_SIZE;i++)
  {
    ptr = HashTable[i];

    if (LowestSoFar == NULLRESET)
      LowestSoFar = ptr;

    while (ptr != NULLRESET) 
    {
      if (ptr->ScoreDepth < LowestSoFar->ScoreDepth)
        LowestSoFar = ptr;
      ptr = ptr->Sibling;
    }
  }
  DuplicateReset(LowestReset,LowestSoFar);
}

#endif
