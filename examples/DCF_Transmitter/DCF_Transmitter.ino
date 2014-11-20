#include <Dcf77Transmitter.h>

#define DCFOutputPin 23

DCF77Transmitter DCF77Transmitter(DCFOutputPin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DCF77Transmitter.blink13();
  //DCF77Transmitter.createtimearray(21, 56, 25, 7, 2014);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int k = 0; k < 20; k++){
    if (k > 9) {
      DCF77Transmitter.createtimearray(14, k - 10, 19, 7, 2014);
      DCF77Transmitter.switch_to_reservantenna();
      //DCF77Transmitter.skip_stream_startbit();
      
//      if (random(4) == 3)
//        DCF77Transmitter.skip_parity(P1);
//      if (random(4) == 3)
//        DCF77Transmitter.skip_parity(P2);
//      if (random(4) == 3)
//        DCF77Transmitter.skip_parity(P3);
//      if (random(4) == 3)
//        DCF77Transmitter.skip_timezone(random(2));
      
      DCF77Transmitter.sendstream(true);
    } else {
      DCF77Transmitter.createtimearray(13, 50 + k, 19, 7, 2014);
      DCF77Transmitter.sendstream(true);
    }
  }
}
