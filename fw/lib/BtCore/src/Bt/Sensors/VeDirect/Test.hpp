#include <SoftwareSerial.h>

long mainOldMillis = millis();

// the Victron Hex Protocol is ASCII representation of Hex, not Hex

// avoided any Arduino String functions. Use char arrays throughout. Arduino adds ‘\0’ to signify end of string.
char receivedString[25];
int receivedStringLength = 0;

// all communication is ascii, for Victron its ascii representation of hex, not hex
// part of simple conversion from byte to ascii hex
char hexChars[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

// Victron MPPT hex protocol commands
// has newline which Victron expects, use print as println without  ‘\n’ produces unreliable responses
char getMPPTChargerVolts[12] = ":7D5ED008C\n";
char getMPPTChargerCurrent[12] = ":7D7ED008A\n";
char getMPPTPanelVolts[12] = ":7BBED00A6\n";
char getMPPTPanelPower[12] = ":7BCED00A5\n";
char getMPPTDeviceMode[14] = ":7000200004C\n";
char getMPPTDeviceState[12] = ":70102004B\n";

char getMPPTMaxCurrent[12] = ":7F0ED0071\n";
char getMPPTBatterySafeMode[12] = ":7FFED0062\n";
char getMPPTExtraInfo[12] = ":7D4ED008D\n";
char getMPPTAbsorptionVolts[12] = ":7F7ED006A\n";

// MPPT On/Off commands, needs at least one enable before on/off works.
char enableMPPTRemoteOnOff[20] = ":80202000200000047\n";
char setMPPTDeviceModeOff[14] = ":80002000447\n";
char setMPPTDeviceModeOn[14] = ":8000200014A\n";
char setMPPTRestart[6] = ":64F\n";

// Victron BM hex protocol commands
char getBMVMainVolts[12] = ":78DED00D4\n";
char getBMVCurrent[12] = ":78FED00D2\n";
char getBMVSOC[12] = ":7FF0F0040\n";

// Data
// change numMPPT to number of units, and change array initialisation
const int numMPPT = 3;
int panelVolts[numMPPT] = {0,0,0};
int panelPower[numMPPT] = {0,0,0};
int chargerVolts[numMPPT] = {0,0,0};
int chargerAmps[numMPPT] = {0,0,0};
int chargerMode[numMPPT] = {0,0,0};
int chargerState[numMPPT] = {0,0,0};

// change numBMV to number of units, and change array initialisation
const int numBMV = 1;
int batteryVolts[numBMV] = {0};
long batteryAmps[numBMV] = {0};
int batterySOC[numBMV] = {0};

// change numCells to number of cells in battery
const int numCells = 4;
int cellVolts[numCells] = {0,0,0,0};
char cellBalance[numCells] = {'o','o','o','o'};
char cellAlarm[numCells] = {'o','o','o','o'};

// change to number of BMS in system, the BMS can be cascaded with 6 cells per unit to a couple of hundred.
const int numBMS = 1;

// array of SoftwareSerial to at least generalise a bit
SoftwareSerial MPPTs[numMPPT] = { SoftwareSerial(12, 13), SoftwareSerial(10, 11), SoftwareSerial(8, 9) };
SoftwareSerial BMVs[numBMV] = { SoftwareSerial(6,7)};
//SoftwareSerial BMSs[numBMS] = { SoftwareSerial(10, 11)};
//SoftwareSerial Monitor = SoftwareSerial(12,13);

void setup() {

  Serial.begin(19200);

// Victrons are 19200 baud, all other settings default
  for (int i=0; i<numMPPT; i++) {MPPTs[i].begin(19200); delay(100);}
  for (int i=0; i<numBMV; i++) {BMVs[i].begin(19200); delay(100);}
  //for (int i=0; i<numBMS; i++) {BMSs[i].begin(19200); delay(100);}
  //Monitor.begin(19200);
  delay(100);

}

void loop() {

  // loops through each Victron unit, which in turn reads data
  mainOldMillis = millis();

  for (int i=0; i<numMPPT; i++) {
    readMPPT(i);
  }

  for (int i=0; i<numBMV; i++) {
    readBMV(i);
  }

  // outputs data to Serial monitor
  printToExcel();

  // allows requests to be made through serial monitor
  checkRequests();

}

void readMPPT(int aMPPT) {

// requests for data from MPPT
// readVictron returns Boolean (success/fail) but not yet used here

  MPPTs[aMPPT].listen();
  delay(10);

  // get Volts
  readVictron(MPPTs[aMPPT], getMPPTChargerVolts, 14, chargerVolts[aMPPT], 10, 1, false);

  // get Amps
  readVictron(MPPTs[aMPPT], getMPPTChargerCurrent, 14, chargerAmps[aMPPT], 100, 1, false);

  // get Panel Volts
  readVictron(MPPTs[aMPPT], getMPPTPanelVolts, 14, panelVolts[aMPPT], 10, 1, false);

  // get Panel Power  (untested, panel amps not available with 75/15)
  //readVictron(MPPTs[aMPPT], getMPPTPanelPower, 14, panelPower[aMPPT]);

  // get State
  readVictron(MPPTs[aMPPT], getMPPTDeviceState, 12, chargerState[aMPPT], 1, 1, false);

  // get Mode
  readVictron(MPPTs[aMPPT], getMPPTDeviceMode, 12, chargerMode[aMPPT], 1, 1, false);

}

void readBMV(int aBMV) {

  BMVs[aBMV].listen();

  // get Volts
  readVictron(BMVs[aBMV], getBMVMainVolts, 14, batteryVolts[aBMV], 10, 1, false);

  // get Amps
  int anotherValue = 0;
  if (readVictron(BMVs[aBMV], getBMVCurrent, 14, anotherValue, 1, 1, false)) {
   // readVictron needs value as int, batteryAmps is uniquely long and may be –ve
   batteryAmps[aBMV] = anotherValue * 100;
  }

  // get SOC
  readVictron(BMVs[aBMV], getBMVSOC, 14, batterySOC[aBMV], 1, 1, false);

}

void clearReceivedString() {
  // just in case, clear string before use
  for (int i=0; i<25; i++) {
    receivedString[i] = ' ';
  }
  receivedString[24] = '\0';
}

boolean readVictron(SoftwareSerial &Victron, char* request, int expectedLength, int &value, int multiplier, int tries, boolean printRead) {

  // the guts of the communication with Victron, send message and process response

  clearReceivedString();
  char characterRead;
  int charNumber = 0;

  // set true when ‘:’ read
  boolean startFound = false;
  long oldMillis = millis();

  // count number of tries
  int counterTries = 0;

  // count number of characters received
  int counterChars = 0;

  // for return
  int success = false;

  // printRead is rudimentary debug so we can see what is happening
  if (printRead){Serial.println(request);}

  // main send/receive loop
  while (counterTries < tries && success == false) {
    oldMillis = millis();

    // send request to Victron
    Victron.print(request);
    //delay(1);
    if (expectedLength > 0) {
      // keep listening to Victron for up to 15mS
      while (millis() - oldMillis < 15 && millis() > oldMillis) {
        counterChars = 0;
        while (Victron.available()) {
          characterRead = Victron.read();
          counterChars += 1;
          if (printRead){Serial.write(characterRead);}
          if (characterRead == ':') {
            if (printRead){Serial.print(" start ");}
            startFound = true;
            charNumber = 0;
            receivedString[charNumber] = characterRead;
            charNumber = 1;
          }
          else if ((characterRead == '\n' && startFound == true) || charNumber > expectedLength) {
            receivedString[charNumber] = characterRead;
            charNumber += 1;
            if (characterRead == '\n' && startFound == true){
              if (printRead){Serial.println("Success");}
              success = true;
              break;
            }
            if (printRead){Serial.println(" line end or count ");}
          }
          else if (startFound) {
            receivedString[charNumber] = characterRead;
            charNumber += 1;
          }
          else if (counterChars > 20) {
            // avoid receivedString overflow
            if (printRead){Serial.println(" No start or end ");}
            break;
          }
        }
        // 25msec should be enough but no need to wait
        if (charNumber > expectedLength){break;}
      }
    }
    counterTries += 1;
    if (printRead){Serial.print(counterTries); Serial.println(" Tries ");}
  }

  // response has been received, now check it

  // is response expected length, checksum correct, matches query
  if (charNumber - 1 == expectedLength){
    // check checksum
    if (victronCheckSum(receivedString,expectedLength)) {
      // Victron response contains request register
      boolean validAnswer = true;
      for (int i=2; i<6; i++) {
        if (request[i] != receivedString[i]){
          validAnswer = false;
          return validAnswer;
        }
      }
      // response is correct for request so convert ascii hex to integer and apply multiplier
      if (validAnswer) {
        if (expectedLength == 14) {
          value = (x2i(&receivedString[10], 2) * 256 + x2i(&receivedString[8], 2));
          value = value  * multiplier;
          return true;
        }
        else if (expectedLength == 12) {
          value = x2i(&receivedString[8], 2);
          value = value  * multiplier;
          return true;
        }
        else if (expectedLength == 10) {
          value = x2i(&receivedString[6], 2);
          value = value  * multiplier;
          return true;
        }
        else if (expectedLength == 18) {
          // the response to enableOnOff is unique
          value = 0;
          return true;
        }
        else {
          // response doesn't match request
          if (printRead){Serial.println("invalid");}
          return false;
        }
      }
    }
  }
  else {
    // wrong length
    if (printRead){Serial.print(charNumber); Serial.println(" wrong length");}
    return false;
  }
}

boolean victronCheckSum(char* testString, int testStringLength) {

  // checks existing check sum
  // using byte instead of int avoids explicitly wrapping checkSum as it overflows
  byte checkSum = 85;

  checkSum = checkSum - x2i(&testString[1], 1);

  for (int i=2; i<testStringLength; i+=2) {
     checkSum = checkSum - x2i(&testString[i], 2);
  }

  if (checkSum == 0) {
    return true;
  }
  else {
    return false;
  }
}

boolean victronCheckSumCalculate(char* testString, int testStringLength) {

  // use to calculate check sum and insert in testString
  byte checkSum = 85;

  checkSum = checkSum - x2i(&testString[1], 1);

  for (int i=2; i<testStringLength - 2; i+=2) {
     checkSum = checkSum - x2i(&testString[i], 2);
  }

  testString[testStringLength-2] = hexChars[(int) floor(checkSum / 16)];
 testString[testStringLength-1] = (int)hexChars[checkSum - 16*(int) floor(checkSum / 16)];
}

void printToExcel() {

  // sends data to serial, formatted so excel can deal with it directly – needs PLX-DAQ activeX

  Serial.print("DATA,TIME,");
  Serial.print(millis()/1000);
  //Serial.print(millis());
  Serial.print(",");
  int totalSolarAmps = 0;
  double minutes = (double)millis() / 60000;
  for (int i=0; i<numMPPT; i++) {
    Serial.print(chargerVolts[i]);
    Serial.print(",");
    Serial.print(chargerAmps[i]);
    Serial.print(',');
    Serial.print(panelVolts[i]);

    Serial.write(',');

    Serial.print(chargerState[i]);
    Serial.write(',');
    Serial.print(chargerMode[i]);
    Serial.write(',');

    totalSolarAmps += chargerAmps[i];
  }
  for (int i=0; i<numBMV; i++) {
    Serial.print(batteryVolts[i]);
    Serial.write(',');
    Serial.print(batteryAmps[i]);
    Serial.write(',');
    Serial.print(batterySOC[i]);
    Serial.write(',');
  }

  for (int i=0; i<numCells; i++) {
    Serial.print(cellVolts[i]);
    Serial.write(',');
  }

  printDouble(minutes,3);
  Serial.write(',');
  Serial.println(totalSolarAmps);
}

void checkRequests() {

  // looks for anything received from Serial monitor
  // format is na where n is the index for array of MPPTs and a is the command
  // for example, 1s gets the device state from MPPT number one
  // sends it to relevant MPPT
  char newRequest[10];
  int charCounter = 0;
  while (Serial.available()) {
    newRequest[charCounter] = Serial.read();
    //Serial.write(Serial.read());
    charCounter += 1;
  }
  if (charCounter > 0){
    int aMPPT = x2i(&newRequest[0], 1);
    Serial.print(aMPPT);
    int aValue = 0;
    MPPTs[aMPPT].listen();
    if (newRequest[1] == '-') {
      Serial.print('-');
      readVictron(MPPTs[aMPPT], enableMPPTRemoteOnOff, 18, aValue, 1, 1, true);
      readVictron(MPPTs[aMPPT], setMPPTDeviceModeOff, 12, chargerMode[aMPPT], 1, 1, true);
    }
    if (newRequest[1] == '+') {
      Serial.print('+');
      readVictron(MPPTs[aMPPT], enableMPPTRemoteOnOff, 18, aValue, 1, 1, true);
      readVictron(MPPTs[aMPPT], setMPPTDeviceModeOn, 12, chargerMode[aMPPT], 1, 1, true);

    }
    if (newRequest[1] == 'r') {
      Serial.print('r');
      readVictron(MPPTs[aMPPT], setMPPTRestart, 0, aValue, 1, 1, true);
    }
    if (newRequest[1] == 'm') {
      Serial.print('m');
      readVictron(MPPTs[aMPPT], getMPPTDeviceMode, 12, chargerMode[aMPPT], 1, 5, true);
    }
    if (newRequest[1] == 's') {
      Serial.print('s');
      readVictron(MPPTs[aMPPT], getMPPTDeviceState, 12, chargerState[aMPPT], 1, 5, true);
      Serial.println(aValue);
    }
    if (newRequest[1] == 'c') {
      Serial.print('c');
      readVictron(MPPTs[aMPPT], getMPPTMaxCurrent, 14, aValue, 100, 5, true);
      Serial.println(aValue);
    }
    if (newRequest[1] == 'b') {
      Serial.print('b');
      //readVictron(MPPTs[aMPPT], getMPPTBatterySafeMode, 12, aValue, 1, 5, true);
      //Serial.println(aValue);
      //readVictron(MPPTs[aMPPT], getMPPTExtraInfo, 12, aValue, 1, 5, true);
      //Serial.println(aValue);
      readVictron(MPPTs[aMPPT], getMPPTAbsorptionVolts, 14, aValue, 10, 5, true);
      Serial.println(aValue);
    }
  }
}

// borrowed from http://forum.arduino.cc/index.php?topic=123486.0
// converts ascii hex to integer
// use instead of strtoul so can refer to chars in a char array
// modified to limit length of "substring" of char array and avoid overflows
byte x2i(char *s, byte numChars)
{
byte x = 0;
for(byte i=0; i<numChars; i++) {
   char c = *s;
   if (c >= '0' && c <= '9') {
      x *= 16;
      x += c - '0';
   }
   else if (c >= 'A' && c <= 'F') {
      x *= 16;
      x += (c - 'A') + 10;
   }
   else if (c >= 'a' && c <= 'f') {
      x *= 16;
      x += (c - 'a') + 10;
   }
   else break;
   s++;
}
return x;
}

void printDouble( double val, byte precision){
// prints val with number of decimal places determine by precision
// precision is a number from 0 to 6 indicating the desired decimial places
// example: printDouble( 3.1415, 2); // prints 3.14 (two decimal places)

Serial.print (int(val));  //prints the int part
if( precision > 0) {
   Serial.print("."); // print the decimal point
   unsigned long frac;
   unsigned long mult = 1;
   byte padding = precision -1;
   while(precision--)
      mult *=10;

   if(val >= 0)
     frac = (val - int(val)) * mult;
   else
     frac = (int(val)- val ) * mult;
   unsigned long frac1 = frac;
   while( frac1 /= 10 )
     padding--;
   while(  padding--)
     Serial.print("0");
   Serial.print(frac,DEC) ;
}
}
