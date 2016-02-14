#include "chess.h"

Lock::Lock()		//constructor
{
  pthread_mutex_init(&MyLock, NULL);
}


void Lock::Obtain()
{
  pthread_mutex_lock(&MyLock);  
}

void Lock::Release()
{
  pthread_mutex_unlock(&MyLock);
}
