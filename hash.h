
extern int HashMultiples[64];

class HashTable 
{
/*************************************************************/
/*************************************************************/
/************************ PRIVATE ****************************/
/*************************************************************/
/*************************************************************/
private:

  ResetTree *Table[HASH_TABLE_SIZE];
  unsigned long int EntryCount;
  unsigned long int MaxEntryCount;
  unsigned long int ReducedEntryCount;
  static int TableCount;

/*************************************************************/
/*************************************************************/
/************************* PUBLIC ****************************/
/*************************************************************/
/*************************************************************/
public:

  HashTable()	//constructor
  {
    int i,j;

    if (TableCount > 0)
    {
      cout << "ERROR!  TOO MANY HASH TABLES!" << endl;
      exit(1);
    }
      
    printf("HashTable Constructor\n");

    //Set up the counts
    EntryCount = 0;
    MaxEntryCount = (HASH_TABLE_MAX_SIZE * ONE_MB) / sizeof(ResetTree);
    ReducedEntryCount = (HASH_TABLE_REDUCED_SIZE * ONE_MB) / sizeof(ResetTree);

    //Fill the table with null pointers
    for (i=0;i<HASH_TABLE_SIZE;i++)
      Table[i] = (ResetTree *) 0;

    //Compute 2^i for 0<=i<=63
    for (i=0;i<64;i++)
    {
      HashMultiples[i] = 1;
      for (j=0;j<i;j++)
        HashMultiples[i] = (HashMultiples[i] * 2) % LARGE_PRIME;
    }
  }

  void Clear()
  {
    int i;

    for (i=0;i<HASH_TABLE_SIZE;i++)
    {
      while (Table[i])
        Table[i]->DeleteFromSiblingList(&(Table[i]),0);
    }
    EntryCount = 0;
  }
};

