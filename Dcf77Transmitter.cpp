#include "Dcf77Transmitter.h"

DCF77Transmitter::DCF77Transmitter(uint8_t pin, boolean mode) {
	DCFOutputPin = pin;
	pinMode(DCFOutputPin, OUTPUT);
  if (mode) {  //NORMAL
    Signal_normal = LOW;
    Signal_data   = HIGH;
  }
  else {       //INVERTED
    Signal_normal = HIGH;
    Signal_data   = LOW;
  }
  digitalWrite(DCFOutputPin, Signal_normal);
}

void DCF77Transmitter::blink13(boolean doblink) {
	flag = doblink;
}

int DCF77Transmitter::sendzero() {
  digitalWrite(DCFOutputPin, Signal_data);
  if (flag)
  	BLINKLED_ON();  // turn pin 13 LED on
  delay(ZeroTime);
  digitalWrite(DCFOutputPin, Signal_normal);
  if (flag)
  	BLINKLED_OFF();  // turn pin 13 LED off
  delay(BitTime - ZeroTime);
}

int DCF77Transmitter::sendone() {
  digitalWrite(DCFOutputPin, Signal_data);
  if (flag)
  	BLINKLED_ON();  // turn pin 13 LED on
  delay(OneTime);
  digitalWrite(DCFOutputPin, Signal_normal);
  if (flag)
  	BLINKLED_OFF();  // turn pin 13 LED off
  delay(BitTime - OneTime);
}

int DCF77Transmitter::lastsecond() {
  digitalWrite(DCFOutputPin, Signal_normal);
  delay(1000);
}

int DCF77Transmitter::even_parity(int first_bit, int last_bit) {
  int parity = 0;
  for (int n = first_bit; n <= last_bit; n++) {
    if (iDateTime[n] == 1) {
      parity = !parity;
    }
  }
  return parity;
}

int DCF77Transmitter::skip_parity(int m) {
  iDateTime[m]=!iDateTime[m];
}

int DCF77Transmitter::cleararray() {
  for (int i = 0; i < horizon; i++)
    iDateTime[i] = 0;
}

void DCF77Transmitter::createtimearray(int hour, int min, int day, int month, int year, boolean leapsecond) {
  int i, minlow, minhi, hourlow, hourhi, daylow, dayhi, weekday, monthlow, monthhi, yearlow, yearhi;
  
  iDateTime[0] = 0;                                                //Start of new Minute has to be 0
  
  for (i = 1; i < 15; i++)                                         //inside coded Weatherdata set random
    iDateTime[i] = random(2);
  
  i = 15;
  iDateTime[i] = 0; //0=Normal antenna, 1=Backup antenna           //antenne bit #15
  i++;
  iDateTime[i] = summertime(hour, day, month, year, true);    		 //summertime_announcement bit #16
  i++;
  iDateTime[i] = summertime(hour, day, month, year, false);   		 //MESZ/MEZ bits #17 and #18
  i++;
  iDateTime[i] = !iDateTime[i-1];                                  //inverse to bit #17
  i++;
  iDateTime[i] = leapsecond;                                       //leapsecond bit #19 set by flag
  i++;
  iDateTime[i] = 1;                                                //Timetablestartbit #20  (has to be 1)
  
  minlow = min % 10;
  
  i++;  // Minute Einer
  iDateTime[i] = (minlow & 1)>>0;
  i++;
  iDateTime[i] = (minlow & 2)>>1;
  i++;
  iDateTime[i] = (minlow & 4)>>2;
  i++;
  iDateTime[i] = (minlow & 8)>>3;
  
  minhi = min - minlow;
  minhi /= 10;
  
  i++;  // Minute Zehner
  iDateTime[i] = (minhi & 1)>>0;
  i++;
  iDateTime[i] = (minhi & 2)>>1;
  i++;
  iDateTime[i] = (minhi & 4)>>2;
  
  i++;
  iDateTime[i] = even_parity(21, 27);  // P1
  
  hourlow = hour % 10;
  
  i++;  // Stunde Einer
  iDateTime[i] = (hourlow & 1)>>0;
  i++;
  iDateTime[i] = (hourlow & 2)>>1;
  i++;
  iDateTime[i] = (hourlow & 4)>>2;
  i++;
  iDateTime[i] = (hourlow & 8)>>3;
  
  hourhi = hour - hourlow;
  hourhi /= 10;
  
  i++;  // Stunde Zehner
  iDateTime[i] = (hourhi & 1)>>0;
  i++;
  iDateTime[i] = (hourhi & 2)>>1;
  
  i++;
  iDateTime[i] = even_parity(29, 34);  // P2
  
  daylow = day % 10;
  
  i++;  // Tag Einer
  iDateTime[i] = (daylow & 1)>>0;
  i++;
  iDateTime[i] = (daylow & 2)>>1;
  i++;
  iDateTime[i] = (daylow & 4)>>2;
  i++;
  iDateTime[i] = (daylow & 8)>>3;
  
  dayhi = day - daylow;
  dayhi /= 10;
  
  i++;  // Tag Zehner
  iDateTime[i] = (dayhi & 1)>>0;
  i++; 
  iDateTime[i] = (dayhi & 2)>>1;
  
  			// Wochentag
  weekday = calculate_day_of_week(year, month, day);
  
  i++;
  iDateTime[i] = (weekday & 1)>>0;
  i++;
  iDateTime[i] = (weekday & 2)>>1;
  i++;
  iDateTime[i] = (weekday & 4)>>2;
  
  monthlow = month % 10;
  
  i++;  // Monat Einer
  iDateTime[i] = (monthlow & 1)>>0;
  i++;
  iDateTime[i] = (monthlow & 2)>>1;
  i++;
  iDateTime[i] = (monthlow & 4)>>2;
  i++;
  iDateTime[i] = (monthlow & 8)>>3;
  
  monthhi = month - monthlow;
  monthhi /= 10;
  
  i++;  // Monat Zehner
  iDateTime[i] = (monthhi & 1)>>0;
  
  year = year % 2000;
  yearlow = year % 10;
  
  i++;  // Jahr Zehner
  iDateTime[i] = (yearlow & 1)>>0;
  i++;
  iDateTime[i] = (yearlow & 2)>>1;
  i++;
  iDateTime[i] = (yearlow & 4)>>2;
  i++;
  iDateTime[i] = (yearlow & 8)>>3;
  
  yearhi = year - yearlow;
  yearhi /= 10;
  
  i++;  // Jahr Einer
  iDateTime[i] = (yearhi & 1)>>0;
  i++;
  iDateTime[i] = (yearhi & 2)>>1;
  i++;
  iDateTime[i] = (yearhi & 4)>>2;
  i++;
  iDateTime[i] = (yearhi & 8)>>3;
  
  i++;
  iDateTime[i] = even_parity(36, 57); // P3
}

int DCF77Transmitter::calculate_day_of_week (int year, int month, int day) {
  int dow;
  
  if ( month == 1 || month == 2 ) {
    month += 12;
    year--;
  }
  
  dow = (day + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400) % 7;
  
  dow++;
  
  return dow;
}

int DCF77Transmitter::summertime (int hour, int day, int month, int year, boolean nexthourchange) {
  int sundaysommertime, sundaywintertime, sundaysommertimehours, sundaywintertimehours, todayhours;
  int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  
  daysInMonth[1] = daysInMonth[1] + switch_years(year);
  
  year = year % 2000;
  sundaysommertime = 31 - ( 5 + year * 5 / 4 ) % 7;
  sundaywintertime = 31 - ( 2 + year * 5 / 4 ) % 7;
  
  //Summertimebegin in March
  for (int i = 1; i < 2; i++)
    sundaysommertime += daysInMonth[i - 1];
  
  //Wintertimebegin in October
  for (int i = 1; i < 9; i++)
    sundaywintertime += daysInMonth[i - 1];
  
  for (int i = 1; i < (month - 1); ++i)
    day += daysInMonth[i - 1];
  
  sundaysommertimehours = sundaysommertime * 24 + 2;
  sundaywintertimehours = sundaywintertime * 24 + 3;
  todayhours = day * 24 + hour;
  
  if (nexthourchange){
    if ((todayhours + 1) == sundaysommertimehours || (todayhours + 1) == sundaywintertimehours) {
      return 1;
    } else {
      return 0;
    }
  } else {
    if (todayhours >= sundaysommertimehours && (todayhours + 1) < sundaywintertimehours) {
      return 1;
    } else {
      return 0;
    }
  }
}

int DCF77Transmitter::switch_years (uint8_t year) {
  if ( ((year %4 == 0) && (year % 100 != 0)) || (year % 400 == 0) )
    return 1;
  else
    return 0;
}

void DCF77Transmitter::sendstream(boolean report) {
	if (report)
		Serial.print("DCF Stream: ");
	
  for (int i = 0; i < horizon; i++) {
    if (iDateTime[i])
      sendone();
    else
      sendzero();
    if (report) {
      if (i ==  1 || i == 15 || i == 21 || i == 29 || i == 36 || i == 42 || i == 45 || i == 50)
        Serial.print("-");
      if (i == 28 || i == 35 || i == 58)
        Serial.print("P");
      Serial.print(iDateTime[i]);
  	}
  }
  if (iDateTime[19]) {
    sendzero();
    if (report)
    	Serial.print(!iDateTime[19]);
  }
  lastsecond();
  if (report)
  	Serial.println(" End");
}