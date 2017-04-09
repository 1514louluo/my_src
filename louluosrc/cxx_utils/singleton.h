/*
 * #################################################################################################
 *	License: MIT
 *	Desc: 线程安全的单例类和非拷贝类，这个非本人所写 来自muduo网络库的单例和boost的非拷贝类 -- manialuo
 * #################################################################################################
 */
#ifndef _UTILS_SINGLETON_H
#define _UTILS_SINGLETON_H

#include <pthread.h>
#include <stdlib.h> // atexit

namespace cxx_utils
{
  class Noncopyable
  {
    protected:
      Noncopyable() {}
      ~Noncopyable() {}
    private:  // emphasize the following members are private
      Noncopyable( const Noncopyable& );
      const Noncopyable& operator=( const Noncopyable& );
  };  

  template<typename T>
  class Singleton : Noncopyable
  {
    public:
      static T& instance()
      {
        pthread_once(&ponce_, &Singleton::init);
        return *value_;
      }

    private:
      Singleton();
      ~Singleton();

      static void init()
      {
        value_ = new T();
        ::atexit(destroy);
      }

      static void destroy()
      {
        delete value_;
      }

    private:
      static pthread_once_t ponce_;
      static T*             value_;
  };

  template<typename T>
  pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

  template<typename T>
  T* Singleton<T>::value_ = NULL;

}
#endif //_UTILS_SINGLETON_H

