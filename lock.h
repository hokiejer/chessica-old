
class Lock {

/*************************************************************/
/*************************************************************/
/*********************** PROTECTED ***************************/
/*************************************************************/
/*************************************************************/
protected:


/*************************************************************/
/*************************************************************/
/************************ PRIVATE ****************************/
/*************************************************************/
/*************************************************************/
private:

  pthread_mutex_t MyLock;


/*************************************************************/
/*************************************************************/
/************************ PUBLIC *****************************/
/*************************************************************/
/*************************************************************/
public:

  //lock.cpp
  Lock();
  void Obtain();
  void Release();
  
};


