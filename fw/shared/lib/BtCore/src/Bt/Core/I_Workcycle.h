//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Core::I_Workcycle
//
//*************************************************************************************************

#ifndef INC__Bt_Core_I_Workcycle__h
#define INC__Bt_Core_I_Workcycle__h

#include <type_traits>

#include "Bt/Core/I_Runnable.h"

namespace Bt {
namespace Core {

/*
class I_Workcycle;

template<typename T, typename C> struct RunnableHandler;
template<typename T> struct RunnableHandler<T,std::true_type> {
   static void add(I_Workcycle& pWorkcycle, T& pRunnable);
   static void remove(I_Workcycle& pWorkcycle, T& pRunnable);
};
template<typename T> struct RunnableHandler<T,std::false_type> {
   static void add(I_Workcycle& pWorkcycle, T& pRunnable);
   static void remove(I_Workcycle& pWorkcycle, T& pRunnable);
};

template<typename T, typename C> struct SchedulingListenerHandler;
template<typename T> struct SchedulingListenerHandler<T,std::true_type> {
   static void add(I_Workcycle& pWorkcycle, T& pSchedulingListener);
   static void remove(I_Workcycle& pWorkcycle, T& pSchedulingListener);
};
template<typename T> struct SchedulingListenerHandler<T,std::false_type> {
   static void add(I_Workcycle& pWorkcycle, T& pSchedulingListener);
   static void remove(I_Workcycle& pWorkcycle, T& pSchedulingListener);
};
*/

class I_Workcycle {
   public:
      virtual ~I_Workcycle() {}

      virtual void add(I_Runnable& iRunnable) = 0;
      virtual void remove(I_Runnable& iRunnable) = 0;

      /*
      virtual void addSchedulingListener(I_SchedulingListener& pSchedulingListener)= 0;
      virtual void removeSchedulingListener(I_SchedulingListener& pSchedulingListener)= 0;

      template<typename T> void add(T& pT) {
         RunnableHandler<T,typename std::is_assignable<I_Runnable,T>::type>::add(*this,pT);
         SchedulingListenerHandler<T,typename std::is_assignable<I_SchedulingListener,T>::type>::add(*this,pT);
      }

      template<typename T> void remove(T& pT) {
         RunnableHandler<T,typename std::is_assignable<I_Runnable,T>::type>::remove(*this,pT);
         SchedulingListenerHandler<T,typename std::is_assignable<I_SchedulingListener,T>::type>::remove(*this,pT);
      }
      */

   private:
      
};

/*

template<typename T>
void RunnableHandler<T,std::true_type>::add(I_Workcycle& pWorkcycle, T& pRunnable){
   pWorkcycle.addRunnable(pRunnable);   
}

template<typename T>
void RunnableHandler<T,std::false_type>::add(I_Workcycle& pWorkcycle, T&pRunnable){
}

template<typename T>
void RunnableHandler<T,std::true_type>::remove(I_Workcycle& pWorkcycle, T& pRunnable){
   pWorkcycle.removeRunnable(pRunnable);   
}

template<typename T>
void RunnableHandler<T,std::false_type>::remove(I_Workcycle& pWorkcycle, T& pRunnable){
}

template<typename T>
void SchedulingListenerHandler<T,std::true_type>::add(I_Workcycle& pWorkcycle, T& pSchedulingListener){
   pWorkcycle.addSchedulingListener(pSchedulingListener);   
}

template<typename T>
void SchedulingListenerHandler<T,std::false_type>::add(I_Workcycle& pWorkcycle, T&pSchedulingListener){
}

template<typename T>
void SchedulingListenerHandler<T,std::true_type>::remove(I_Workcycle& pWorkcycle, T& pSchedulingListener){
   pWorkcycle.removeSchedulingListener(pSchedulingListener);   
}

template<typename T>
void SchedulingListenerHandler<T,std::false_type>::remove(I_Workcycle& pWorkcycle, T& pSchedulingListener){
}
*/

} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_I_Workcycle__h
