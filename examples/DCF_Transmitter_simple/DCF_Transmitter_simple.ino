#include <Dcf77Transmitter.h>

#define DCFOutputPin 23

DCF77Transmitter DCF77Transmitter(DCFOutputPin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DCF77Transmitter.blink13();
  //DCF77Transmitter.createtimearray(21, 56, 25, 7, 2014);
  DCF77Transmitter.createtimearray(__TIME__, __DATE__);
  DCF77Transmitter.skip_timezone();
}

void loop() {
  DCF77Transmitter.sendstream(true);
}
