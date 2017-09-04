#include <LiquidCrystal.h>
#include <DFR_LCD_Keypad.h>

// this is factory config shield initialisation

LiquidCrystal lcd(8,9,4,5,6,7);
DFR_LCD_Keypad keypad(A0, &lcd);

unsigned long Watch, _micro, time = micros();
unsigned int Clock = 0, R_clock;
boolean Reset = false, Stop = false, Paused = false;
volatile boolean timeFlag = false;
int last_key, key;

void setup()

{
  SetTimer(0,1,10); // 10 seconds
  StartTimer();
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(5,0);
}

void loop()
{
  CountDownTimer(); // run the timer
  last_key = keypad.get_last_key(); // get last key value
  key      = keypad.read_key(); // read current key value

  // this prevents the time from being constantly shown.
  if (TimeHasChanged() ) 
  {
    lcd.setCursor(5,0);
    lcd.print(ShowHours());
    lcd.print(":");
    lcd.print(ShowMinutes());
    lcd.print(":");
    lcd.print(ShowSeconds());
    //lcd.print(":");
    //lcd.print(ShowMilliSeconds());
    //lcd.print(":");
    //lcd.println(ShowMicroSeconds());
    // This DOES NOT format the time to 0:0x when seconds is less than 10.
    // if you need to format the time to standard format, use the sprintf() function.
  }
}

boolean CountDownTimer()
{
  static unsigned long duration = 1000000; // 1 second
  timeFlag = false;

  if (!Stop && !Paused) // if not Stopped or Paused, run timer
  {
    // check the time difference and see if 1 second has elapsed
    if ((_micro = micros()) - time > duration ) 
    {
      Clock--;
      timeFlag = true;

      if (Clock == 0) // check to see if the clock is 0
        Stop = true; // If so, stop the timer
        
     // check to see if micros() has rolled over, if not,
     // then increment "time" by duration
      _micro < time ? time = _micro : time += duration; 
    }
  }
  return !Stop; // return the state of the timer
}

void ResetTimer()
{
  SetTimer(R_clock);
  Stop = false;
}

void StartTimer()
{
  Watch = micros(); // get the initial microseconds at the start of the timer
  time = micros(); // hwd added so timer will reset if stopped and then started
  Stop = false;
  Paused = false;
}

void StopTimer()
{
  Stop = true;
}

void StopTimerAt(unsigned int hours, unsigned int minutes, unsigned int seconds)
{
  if (TimeCheck(hours, minutes, seconds) )
    Stop = true;
}

void PauseTimer()
{
  Paused = true;
}

void ResumeTimer() // You can resume the timer if you ever stop it.
{
  Paused = false;
}

void SetTimer(unsigned int hours, unsigned int minutes, unsigned int seconds)
{
  // This handles invalid time overflow ie 1(H), 0(M), 120(S) -> 1, 2, 0
  unsigned int _S = (seconds / 60), _M = (minutes / 60);
  if(_S) minutes += _S;
  if(_M) hours += _M;

  Clock = (hours * 3600) + (minutes * 60) + (seconds % 60);
  R_clock = Clock;
  Stop = false;
}

void SetTimer(unsigned int seconds)
{
// StartTimer(seconds / 3600, (seconds / 3600) / 60, seconds % 60);
 Clock = seconds;
 R_clock = Clock;
 Stop = false;
}

int ShowHours()
{
  return Clock / 3600;
}

int ShowMinutes()
{
  return (Clock / 60) % 60;
}

int ShowSeconds()
{
  return Clock % 60;
}

unsigned long ShowMilliSeconds()
{
  return (_micro - Watch)/ 1000.0;
}

unsigned long ShowMicroSeconds()
{
  return _micro - Watch;
}

boolean TimeHasChanged()
{
  return timeFlag;
}

// output true if timer equals requested time
boolean TimeCheck(unsigned int hours, unsigned int minutes, unsigned int seconds) 
{
  return (hours == ShowHours() && minutes == ShowMinutes() && seconds == ShowSeconds());
}
