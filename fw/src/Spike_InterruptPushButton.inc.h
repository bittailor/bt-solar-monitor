#include <functional>
#include <Particle.h>

#include <Bt/Core/Workcycle.h>
#include <Bt/Core/InterruptPushButton.h>


SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

class NeverSleep : public Bt::Core::I_Runnable {
   public:
      virtual Bt::Core::Scheduling workcycle(){
         return Bt::Core::Scheduling::immediately();
      }
};

Serial1LogHandler logHandler(115200,LOG_LEVEL_INFO);

int sBlueLed = 7;

Bt::Core::Workcycle sWorkcycle(A0);
NeverSleep sNeverSleep;
Bt::Core::InterruptPushButton sUp(C5);
Bt::Core::InterruptPushButton sDown(C4);

void setup() {
   Log.info("*** Spike InterruptPushButton ***");
   pinMode(sBlueLed, OUTPUT);

   sWorkcycle.add(sNeverSleep);
   sWorkcycle.add(sUp);
   sWorkcycle.add(sDown);

   sWorkcycle.begin();
   sUp.begin();
   sDown.begin();
}

void loop() {
   //Log.info("loop %d - %d", pinReadFast(C4), pinReadFast(C5) );
   sWorkcycle.oneWorkcycle();
}

