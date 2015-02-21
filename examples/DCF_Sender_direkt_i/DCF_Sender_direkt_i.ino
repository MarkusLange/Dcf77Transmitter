#define DCFOutputPin 13
#define BitTime  1000
#define ZeroTime  100
#define OneTime   200

#define horizon 59

int iDateTime[horizon];

void setup() {
  Serial.begin(9600);
  pinMode(DCFOutputPin,OUTPUT);
  digitalWrite(DCFOutputPin, LOW);
  //createtimearray( min, hour, day, month, year, leapsecond)
  createtimearray(21, 13, 7, 5, 14, false);
}

void loop() {
  Serial.println("Round");  
  sendingdate();
}

void sendingdate(){
  for (int i = 0; i < horizon; i++) {
    if (iDateTime[i] == 1)
      sendone();
    else
      sendzero();
    if (i == 1 || i == 15 || i == 21 || i == 29 || i == 36 || i == 42 || i == 45 || i == 50)
      Serial.print("-");
    Serial.print(iDateTime[i]);
  }
  lastsecond();
  Serial.println("Roundend");
}

int sendzero() {
  digitalWrite(DCFOutputPin, HIGH);
  delay(ZeroTime);
  digitalWrite(DCFOutputPin, LOW);
  delay(BitTime - ZeroTime);
}

int sendone() {
  digitalWrite(DCFOutputPin, HIGH);
  delay(OneTime);
  digitalWrite(DCFOutputPin, LOW);
  delay(BitTime - OneTime);
}

int lastsecond() {
  digitalWrite(DCFOutputPin, LOW);
  delay(1000);
}

int even_parity(int first_bit, int last_bit) {
  int parity = 0;
  for (int n = first_bit; n <= last_bit; n++) {
    if (iDateTime[n] == 1) {
      parity = !parity;
    }
  }
  return parity;
}

int cleararray() {
  for (int i = 0; i < horizon; i++)
    iDateTime[i] = 0;
}

void createtimearray(int min, int hour, int day, int month, int year, boolean leapsecond){
  int i, minlow, minhi, hourlow, hourhi, daylow, dayhi, weekday, monthlow, monthhi, yearlow, yearhi;
  
  iDateTime[0] = 0;    //Start of new Minute has to be 0
  
  for (i = 1; i < 15; i++)    //inside coded Weatherdata sett to 1
    iDateTime[i] = 1;
  
  i = 15;
  iDateTime[i] = 0;    //antenne bit #15
  i++;
  iDateTime[i] = summertime(hour, day, month, year, day, true);    //summertime_announcement bit #16  //0
  i++;
  iDateTime[i] = summertime(hour, day, month, year, day, false);   //MESZ/MEZ bits #17 and #18        //1
  i++;
  iDateTime[i] = !iDateTime[i-1];    //inverse to bit 17                                              //0
  i++;
  iDateTime[i] = leapsecond;    //leapsecond bit #19                                                  //0
  i++;
  iDateTime[i] = 1;    //Timetablestartbit #20  (has to be 1)                                         //1
  
  minlow = min % 10;
  
  i++;  // Minute Einer
  iDateTime[i] = (minlow & 1)>>0;                                                                     //1
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
  iDateTime[i] = (minhi & 2)>>1;                                                                      //1
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
  
  yearlow = year % 10;
  
  i++;  // Jahr Einer
  iDateTime[i] = (yearlow & 1)>>0;
  i++;
  iDateTime[i] = (yearlow & 2)>>1;
  i++;
  iDateTime[i] = (yearlow & 4)>>2;
  i++;
  iDateTime[i] = (yearlow & 8)>>3;
  
  yearhi = year - yearlow;
  yearhi /= 10;
  
  i++;  // Jahr Zehner
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

int calculate_day_of_week (int _year, int _month, int _day) {
  int _week;
  
  if (_month == 1 || _month == 2) {
    _month += 12;
    --_year;
  }
  
  _week = (_day + 2 * _month + 3 * (_month + 1) / 5 + _year + _year / 4 - _year / 100 + _year / 400) % 7;
  
  ++_week;
  
  return _week;
}

int summertime (int _hour, int _day, int _month, int _year, int today, boolean nexthourchange) {
  int sundaysommertime, sundaywintertime, sundaysommertimehours, sundaywintertimehours, todayhours;
  int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  
  sundaysommertime = 31 - ( 5 + _year * 5 / 4 ) % 7;
  sundaywintertime = 31 - ( 2 + _year * 5 / 4 ) % 7;
  
  //Summertimebegin in March
  for (int i = 1; i < 2; ++i)
    sundaysommertime += daysInMonth[i - 1];
  
  //Wintertimebegin in October
  for (int i = 1; i < 9; ++i)
    sundaywintertime += daysInMonth[i - 1];
  
  for (int i = 1; i < (_month - 1); ++i)
    today += daysInMonth[i - 1];
  
  sundaysommertimehours = sundaysommertime * 24 + 2;
  sundaywintertimehours = sundaywintertime * 24 + 3;
  todayhours = today * 24 + _hour;
  
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
