#include <functional>

#include <Particle.h>
#include <Serial4/Serial4.h>
#include <Serial5/Serial5.h>

#include <Bt/Sensors/VeDirect/Registers.h>

using namespace Bt::Sensors::VeDirect;

SYSTEM_MODE(MANUAL);

uint32_t sLoopCounter = 0;
int sBlueLed = 7;

int inIndex = 0;
char inBuffer[1000] = {0};
int outIndex = 0;
char outBuffer[1000] = {0};

int cmdIndex = 0;
char cmdBuffer[1000] = {0};

Stream* pInputStream = 0;

void setup() {
   Log.info("*** VE.Direct Redirect ***");

   Serial.begin(115200);
   Serial4.begin(19200);
   Serial5.begin(19200);
   Serial1.begin(19200);

   pinMode(sBlueLed, OUTPUT);
   digitalWrite(sBlueLed, LOW);

}

void onCommand(const char* pCmd) {
   if(strcmp("off",pCmd) == 0){
      Serial.println("exe>off");
      pInputStream = 0;
      return;
   }
   if(strcmp("on4",pCmd) == 0){
      Serial.println("exe>on4");
      pInputStream = &Serial4;
      return;
   }
   if(strcmp("on5",pCmd) == 0){
      Serial.println("exe>on5");
      pInputStream = &Serial5;
      return;
   }
   if(strcmp("dfu",pCmd) == 0){
         Serial.println("exe>dfu");
         Serial.flush();
         System.dfu();
         return;
   }
   if(strcmp("reset",pCmd) == 0){
         Serial.println("exe>dfu");
         Serial.flush();
         System.reset();
         return;
      }
   Serial.println("fail>unknown command");
}

void loop() {
   if(pInputStream != 0) {
      if (pInputStream->available())
      {
         int inByte = pInputStream->read();
         Serial1.write(inByte);
         if(inByte == '\n') {
            Serial.print("in >");Serial.println(inBuffer);
            inIndex = 0;
         }
         else {
            inBuffer[inIndex] = inByte;
            inIndex++;
         }
         inBuffer[inIndex] = 0;
      }
      if (Serial1.available())
      {
         int inByte = Serial1.read();
         pInputStream->write(inByte);
         if(inByte == '\n') {
            Serial.print("out>");Serial.println(outBuffer);
            outIndex = 0;
         }
         else {
            outBuffer[inIndex] = inByte;
            outIndex++;
         }
         outBuffer[outIndex] = 0;
      }
   }

   if(Serial.available()) {
      int inByte = Serial.read();
      if(inByte == '\n') {
         Serial.print("cmd>'");Serial.print(cmdBuffer);Serial.println("'");
         onCommand(cmdBuffer);
         cmdIndex = 0;
      }
      else {
         cmdBuffer[cmdIndex] = inByte;
         cmdIndex++;
      }
      cmdBuffer[cmdIndex] = 0;
   }
}



