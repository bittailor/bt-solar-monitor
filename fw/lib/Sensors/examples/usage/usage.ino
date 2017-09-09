// Example usage for Sensors library by bittailor.

#include "Sensors.h"

// Initialize objects from the lib
Sensors sensors;

void setup() {
    // Call functions on initialized library objects that require hardware
    sensors.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    sensors.process();
}
