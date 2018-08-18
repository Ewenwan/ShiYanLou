#include <MsTimer2.h>

// Switch on LED on pin 13 each second


void flash() {
  static boolean output = HIGH;
  
  digitalWrite(13, output);
  output = !output;
}

void setup() {
  pinMode(13, OUTPUT);

  MsTimer2::set(500, flash); // 500ms period
  MsTimer2::start();
}

void loop() {
}
