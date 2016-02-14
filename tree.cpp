#include "chess.h"
#include <pthread.h>

ResetTree * MoveHistory[200] = {(ResetTree *) 0}; //Only works for zero
ResetTree * ResetTree::FreeList[MAX_THREADS] = {(ResetTree *) 0}; //Only works for zero
int ResetTree::CeaseProcessing = 0;



ResetTree::ResetTree()	//constructor
{
  NextSibling = (ResetTree *) 0;
  PrevSibling = (ResetTree *) 0;
  Parent = (ResetTree *) 0;
  Children.First = (ResetTree *) 0;
  Children.Last = (ResetTree *) 0;
  BestMoveSaved = 0;
}


void ResetTree::InitFreeList()
{
  int i;
  for (i=0;i<MAX_THREADS;i++)
  {
    FreeList[i] = (ResetTree *) 0;
  }
}


void ResetTree::PopulateFreeList(int ThreadID)
{
  int i;

  for (i=0;i<100;i++)
  {
    ResetTree & NewNode = *new ResetTree;
    NewNode.DeleteMe(ThreadID);
  }
}


int ResetTree::CountFreeList()
{
  int i;
  long int Count;
  ResetTree *Node;

  Count = 0;
  for (i=0;i<MAX_THREADS;i++)
  {
    Node = FreeList[i];
    while (Node)
    {
      Count++;
      Node = Node->NextSibling;
    }
  }
  return (Count);
}


ResetTree * ResetTree::GetNode(int ThreadID)
{
  ResetTree *NewNodePtr;

  //Populate the FreeList if it is empty
  if (FreeList[ThreadID] == (ResetTree *) 0)	//If the FreeList is empty
    PopulateFreeList(ThreadID);			//Add more nodes

  //Remove the first node off of the FreeList
  NewNodePtr = FreeList[ThreadID];		//Use first node on FreeList
  FreeList[ThreadID] = NewNodePtr->NextSibling; 	//Thread FreeList around it

  return(NewNodePtr);				//Return the new node
}


ResetTree * ResetTree::NewOrphan(int ThreadID)
{
  ResetTree *NewNodePtr = GetNode(ThreadID);	//Get a new node

  InitMyChild(NewNodePtr);		//Initialize my child
  NewNodePtr->Parent = this;

  return NewNodePtr;			//Return the child pointer
}


void ResetTree::PutNodeLastInList(ResetTree *Newguy)
{
  Newguy->NextSibling = (ResetTree *) 0;
  if (Children.Last)
  {
    Children.Last->NextSibling = Newguy;
  }
  else //empty list
  {
    Children.First = Newguy;
  }
  Newguy->PrevSibling = Children.Last;		//Point it to its new sibling
  Children.Last = Newguy;			//Make it the new child
}


void ResetTree::PutNodeFirstInList(ResetTree *Newguy)
{
  Newguy->PrevSibling = (ResetTree *) 0;
  if (Children.First)
  {
    Children.First->PrevSibling = Newguy;
  }
  else //empty list
  {
    Children.Last = Newguy;
  }
  Newguy->NextSibling = Children.First;		//Point it to its new sibling
  Children.First = Newguy;			//Make it the new child
}


ResetTree * ResetTree::NewChild(int ThreadID)
{
  ResetTree *NewNodePtr = GetNode(ThreadID);		//Get a new node

  PutNodeLastInList(NewNodePtr);

  InitMyChild(NewNodePtr);			//Initialize my child
  NewNodePtr->Parent = this;

  return NewNodePtr;				//Return the child pointer
}



int ResetTree::CountChildren()
{
  ResetTree *CurrentNode = Children.First;	//Start with the first child
  int count=0;
  while (CurrentNode)				//While there are children
  {
    count++;
    CurrentNode = CurrentNode->NextSibling;
  }
  return(count);
}

int ResetTree::CountTree()
{
  ResetTree *CurrentNode = Children.First;	//Start with the first child
  int count=0;
  while (CurrentNode)				//While there are children
  {
    count++;
    count += CurrentNode->CountTree();
    CurrentNode = CurrentNode->NextSibling;
  }
  return(count);
}


ResetTree * ResetTree::DequeueChild(ResetTree *MyChild)
{
  if (MyChild->PrevSibling)
  {
    MyChild->PrevSibling->NextSibling = MyChild->NextSibling;
  }
  else //First child in list
  {
    Children.First = MyChild->NextSibling;
  }

  if (MyChild->NextSibling)
  {
    MyChild->NextSibling->PrevSibling = MyChild->PrevSibling;
  }
  else //Last child in list
  {
    Children.Last = MyChild->PrevSibling;
  }

  return (MyChild->NextSibling);
}


ResetTree * ResetTree::DequeueChildNotFirst(ResetTree *MyChild)
{
  MyChild->PrevSibling->NextSibling = MyChild->NextSibling;

  if (MyChild->NextSibling)
  {
    MyChild->NextSibling->PrevSibling = MyChild->PrevSibling;
  }
  else //Last child in list
  {
    Children.Last = MyChild->PrevSibling;
  }

  return (MyChild->NextSibling);
}

ResetTree * ResetTree::DequeueChildNotLast(ResetTree *MyChild)
{
  if (MyChild->PrevSibling)
  {
    MyChild->PrevSibling->NextSibling = MyChild->NextSibling;
  }
  else //First child in list
  {
    Children.First = MyChild->NextSibling;
  }

  MyChild->NextSibling->PrevSibling = MyChild->PrevSibling;

  return (MyChild->NextSibling);
}


ResetTree * ResetTree::DequeueFirstChild()
{
  ResetTree *MyChild = Children.First;

  if (MyChild)
  {
    Children.First = MyChild->NextSibling;
  
    if (MyChild->NextSibling)
    {
      MyChild->NextSibling->PrevSibling = MyChild->PrevSibling;
    }
    else //Only child in list
    {
      Children.Last = ((ResetTree *) 0);
    }
    return (MyChild);
  }
  else
    return ((ResetTree *) 0);
}


void ResetTree::DeleteChildren(int ThreadID) 
{
  ResetTree * CurrentNode;

  while (CurrentNode = Children.First)	//While there are children
  {
    //Cut the child loose
    //THIS IS QUICK AND DIRTY
    Children.First = CurrentNode->NextSibling;

    //Add the Child to the FreeList
    CurrentNode->DeleteMe(ThreadID);
  }
  Children.Last = (ResetTree *) 0;
}


void ResetTree::DeleteTree(int ThreadID)
{
  ResetTree * CurrentNode;

  while (CurrentNode = Children.First)		//While there are children
  {
    Children.First->DeleteTree(ThreadID);

    //Cut the child loose
    //THIS IS QUICK AND DIRTY
    Children.First = CurrentNode->NextSibling;

    //Add the Child to the FreeList
    CurrentNode->DeleteMe(ThreadID);
  }
  Children.Last = (ResetTree *) 0;
}


void ResetTree::DeleteMe(int ThreadID)
{
  NextSibling = FreeList[ThreadID];
  FreeList[ThreadID] = this;
}



void ResetTree::InitializeSerialization()
{
  pthread_mutex_init(&Children.Mutex,NULL);
}



void ResetTree::Serialize()
{
  int retcode;

  if (retcode = pthread_mutex_lock(&Children.Mutex))
  {
    printf("LOCKING ERROR!!! %d\n",retcode);
  }
}


int ResetTree::Serialize_Or_Skip()
{
  int retcode;

  retcode = pthread_mutex_trylock(&Children.Mutex);
  if (retcode == 0)
  {
    //printf("trylock worked (%d)!\n",pthread_self());
    return(1);
  }
  else
  {
    //printf("retcode %d from trylock (%d)\n",retcode,pthread_self());
    return(0);
  }
}


void ResetTree::Unserialize()
{
  int retcode;

  if (retcode = pthread_mutex_unlock(&Children.Mutex))
  {
    printf("UNLOCKING ERROR!!! %d\n",retcode);
  }
}


void ResetTree::PrepareToSearch()
{
  CeaseProcessing = 0;
}


int ResetTree::AbortTreeSearch()
{
  if (CeaseProcessing == 0)
  {
    CeaseProcessing = 1;
    return(1); //Actually ceased something
  }
  else
    return(0); //Didn't do anything
}


int ResetTree::StillSearching()
{
  return(!CeaseProcessing);
}


//Returns a pointer to a matching child if there is one
ResetTree * ResetTree::FindMatchingChild(ResetTree *MyNode)
{
  ResetTree *CurrentNode = Children.First;

  while (CurrentNode)
  {
    if ((CurrentNode->To == MyNode->To) && (CurrentNode->From == MyNode->From))
      return (CurrentNode);
    CurrentNode = CurrentNode->NextSibling;
  }
  return 0;
}


//Returns a pointer to a matching child if there is one
ResetTree * ResetTree::FindMatchingChild(char fromsquare, char tosquare)
{
  ResetTree *CurrentNode = Children.First;

  while (CurrentNode)
  {
    if ((CurrentNode->To == tosquare) && (CurrentNode->From == fromsquare))
      return (CurrentNode);
    CurrentNode = CurrentNode->NextSibling;
  }
  return 0;
}


void ResetTree::GetMoveText(char buffer[])
{
  int i = 4;
  unsigned long long int Bitmask = 0x8000000000000000;
  SquareNumberToText(From,&(buffer[0]));
  SquareNumberToText(To,&(buffer[2]));
  if (Promotion)
  {
    if ((Bitmask >> To) & bBishops)
      buffer[i++] = 'b';
    if ((Bitmask >> To) & bKnights)
      buffer[i++] = 'n';
    if ((Bitmask >> To) & bRooks)
      buffer[i++] = 'r';
    if ((Bitmask >> To) & bQueens)
      buffer[i++] = 'q';
  }
  buffer[i] = '\0';
}



void ResetTree::TreeDiagnostics()
{
  ResetTree *NodePtrs[200];
  unsigned long long DepthCount[200];
  unsigned long long TotalCount = 0;
  int i;
  
  for(i=1;i<200;i++)
  {
    DepthCount[i] = 0;
    NodePtrs[i] = (ResetTree *) 0;
  }
  
  i = 0;
  NodePtrs[i] = this;
  do
  {
    if (NodePtrs[i])
    {
      TotalCount++;
      DepthCount[i]++;
      NodePtrs[i+1] = NodePtrs[i]->Children.First;
      i++;
    }
    else {
      i--;
      if (NodePtrs[i])
      {
        NodePtrs[i] = NodePtrs[i]->NextSibling;
      }
    }  
  } while (i>0);

  for(i=1;i<200;i++)
  {
    if (DepthCount[i] > 0)
    {
      printf("Depth = %d, Nodes = %d\n",i,DepthCount[i]);
    }
  }
  printf("TOTAL = %d\n",TotalCount);
  i = CountFreeList();
  printf("Nodes on Freelist: %d\n",i);
  printf("MEMORY USAGE:\n");
  printf("=============\n");
  printf("Freelist: %6.2f MB (%d nodes)\n",((float) (i) * sizeof(ResetTree)) / (1024.0 * 1024.0),i);
  printf("Tree:     %6.2f MB (%d nodes)\n",((float) (TotalCount) * sizeof(ResetTree)) / (1024.0 * 1024.0),TotalCount);
  printf("TOTAL:    %6.2f MB (%d nodes)\n",((float) (TotalCount + i) * sizeof(ResetTree)) / (1024.0 * 1024.0),TotalCount + i);
}

