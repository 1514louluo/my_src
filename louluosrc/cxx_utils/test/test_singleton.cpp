#include "singleton.h"
#include <iostream>

using namespace std;
using namespace music_util;

class A
{
  public:
    A(){}
    ~A(){}
  public:
    string  m_str;
    int     m_int;
};

static void *do_in_thread(void *arg)
{
  A &a = Singleton<A>::instance();
  cout<<a.m_str<<endl;
  cout<<a.m_int<<endl;
  return NULL;
}

int main()
{
  A &a = Singleton<A>::instance();
  a.m_str = "12345";
  a.m_int = 12345;
  cout<<a.m_str<<endl;
  cout<<a.m_int<<endl;


  A &a1 = Singleton<A>::instance();
  cout<<a1.m_str<<endl;
  cout<<a1.m_int<<endl;

  a.m_str = "54321";
  a.m_int = 54321;


  A &a2 = Singleton<A>::instance();
  cout<<a2.m_str<<endl;
  cout<<a2.m_int<<endl;

  pthread_t tid;
  int result = pthread_create(&tid, NULL, do_in_thread, NULL);
  if (result == -1) 
  {   
    return -1;
  }  
  pthread_join(tid,NULL);

  return 0;
}
