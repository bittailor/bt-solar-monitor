//*************************************************************************************************
//
//  BITTAILOR.CH - Bt::Core::Workcycle
//
//*************************************************************************************************

#ifndef INC__Bt_Core_Workcycle__h
#define INC__Bt_Core_Workcycle__h

#include <vector>
#include "Bt/Core/I_Workcycle.h"

namespace Bt {
namespace Core {

class Workcycle : public I_Workcycle
{
   public:
      Workcycle();
      ~Workcycle();

      virtual void add(I_Runnable& iRunnable);
      virtual void remove(I_Runnable& iRunnable);

      void oneWorkcycle();

   private:
        // Constructor to prohibit copy construction.
      Workcycle(const Workcycle&);

      // Operator= to prohibit copy assignment
      Workcycle& operator=(const Workcycle&);

      void scheduling(Scheduling pScheduling);

      typedef std::vector<I_Runnable*> Runnables;

      Runnables mRunnables;
};


} // namespace Core
} // namespace Bt

#endif // INC__Bt_Core_Workcycle__h
