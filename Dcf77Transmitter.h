#ifndef DCF77Transmitter_H
#define DCF77Transmitter_H

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

//#define DEBUG
//#define SUPER_DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(x)		Serial.print (x)
  #define DEBUG_PRINTDEC(x)	Serial.print (x, DEC)
  #define DEBUG_PRINTHEX(x)	Serial.print (x, HEX)
  #define DEBUG_PRINTLN(x)	Serial.println (x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTDEC(x)
  #define DEBUG_PRINTHEX(x)
  #define DEBUG_PRINTLN(x)
#endif

#define BitTime  1000
#define ZeroTime  100
#define OneTime   200

#define horizon 59
#define P1 			28
#define P2			35
#define P3 			58

class DCF77Transmitter {
 private:
 	int DCFOutputPin;
 	int iDateTime[horizon];
 	int flag;
	boolean Signal_normal, Signal_data;
	int sendzero ();
	int sendone ();
	int lastsecond ();
	int even_parity (int first_bit, int last_bit);
	int calculate_day_of_week (int _year, int _month, int _day);
	int summertime (int hour, int day, int month, int year, boolean nexthourchange);
	int switch_years (uint8_t year);
	
 public:
  DCF77Transmitter (uint8_t pin, boolean mode = true);
  void createtimearray (int hour, int min, int day, int month, int year, boolean leapsecond = false);
  int cleararray ();
  int skip_parity (int m);
  void sendstream (boolean report = false);
  void blink13 (boolean doblink = true);
  
};

// defines for blinking the LED
#if defined(LED_BUILTIN)
  #define BLINKLED       LED_BUILTIN
  #define BLINKLED_ON()  (digitalWrite(LED_BUILTIN, HIGH))
  #define BLINKLED_OFF() (digitalWrite(LED_BUILTIN, LOW))
#elif defined(CORE_LED0_PIN)
  #define BLINKLED       CORE_LED0_PIN
  #define BLINKLED_ON()  (digitalWrite(CORE_LED0_PIN, HIGH))
  #define BLINKLED_OFF() (digitalWrite(CORE_LED0_PIN, LOW))
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	#define BLINKLED       13
	#define BLINKLED_ON()  (PORTB |= B10000000)
	#define BLINKLED_OFF() (PORTB &= B01111111)
#elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__)
	#define BLINKLED       0
	#define BLINKLED_ON()  (PORTD |= B00000001)
	#define BLINKLED_OFF() (PORTD &= B11111110)
#elif defined(__AVR_ATmega32U4__) && defined(IR_USE_TIMER4_HS)
	#define BLINKLED       1
	#define BLINKLED_ON()  (digitalWrite(BLINKLED, HIGH))
	#define BLINKLED_OFF() (digitalWrite(BLINKLED, LOW))
#elif defined(__MK20DX128__) || defined(__MK20DX256__)
	#define BLINKLED       13
	#define BLINKLED_ON()  (digitalWrite(BLINKLED, HIGH))
	#define BLINKLED_OFF() (digitalWrite(BLINKLED, LOW))
#else
	#define BLINKLED       13
	#define BLINKLED_ON()  (PORTB |= B00100000)
	#define BLINKLED_OFF() (PORTB &= B11011111)
#endif

#endif