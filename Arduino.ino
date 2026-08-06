#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

//---------------BUTTONS-----------------
#define MENU_BTN    2
#define UP_BTN      3
#define DOWN_BTN    4
#define SELECT_BTN  5

//---------------OUTPUTS-----------------
#define BUZZER      8
#define FLASHLIGHT  13

//---------------MENU--------------------
int menuIndex = 0;

const int totalMenus = 8;

String menus[totalMenus]={
"Temperature",
"Timer",
"Stopwatch",
"Homework",
"Quotes",
"Settings",
"Flashlight",
"About"
};

//-----------Custom Characters------------

//Open Eye
byte eyeOpen[8]={
B00000,
B01110,
B10001,
B10101,
B10001,
B01110,
B00000,
B00000
};

//Closed Eye
byte eyeClosed[8]={
B00000,
B00000,
B11111,
B00000,
B00000,
B00000,
B00000,
B00000
};

bool blink=false;
unsigned long previousBlink=0;


//-------------Second Part---------
//-------------TEMPERATURE----------------
#define TEMP_PIN A0

float temperature = 0;

//-------------TIMER----------------------
bool timerRunning = false;
unsigned long timerStart = 0;
unsigned long timerDuration = 60000;   // 1 minute

//-------------STOPWATCH------------------
bool stopwatchRunning = false;
unsigned long stopwatchStart = 0;
unsigned long stopwatchElapsed = 0;


//--------------Third Part----------------
//-------------Reminders---------

//-------------REMINDERS-----------------
unsigned long waterTimer = 0;
unsigned long stretchTimer = 0;
unsigned long sleepTimer = 0;
unsigned long homeworkTimer = 0;

const unsigned long WATER_INTERVAL = 300000UL;      //5 min (change later)
const unsigned long STRETCH_INTERVAL = 600000UL;    //10 min
const unsigned long HOMEWORK_INTERVAL = 900000UL;   //15 min
const unsigned long SLEEP_INTERVAL = 1200000UL;     //20 min

bool reminderEnable = true;

//---------Quotes -----------

String quotes[]={
"Believe!",
"Never Give Up",
"You Can Do It",
"Dream Big",
"Study Smart",
"Stay Focused",
"Keep Learning",
"Work Hard",
"Success Awaits",
"Keep Smiling"
};

const int totalQuotes=10;


//--------------First part------------

void beep()
{
  tone(BUZZER,1200,80);
}

void drawEyes()
{
  lcd.setCursor(0,0);

  if(blink)
  {
    lcd.write(byte(1));
    lcd.setCursor(15,0);
    lcd.write(byte(1));
  }
  else
  {
    lcd.write(byte(0));
    lcd.setCursor(15,0);
    lcd.write(byte(0));
  }
}

void showMenu()
{
  lcd.clear();

  drawEyes();

  lcd.setCursor(2,0);
  lcd.print("MENU");

  lcd.setCursor(0,1);
  lcd.print(">");

  lcd.print(menus[menuIndex]);
}

//------------second part ------------
void showTemperature()
{
  lcd.clear();

  while(true)
  {
    int value = analogRead(TEMP_PIN);

    temperature = (value * 100.0) / 3504.0;

    drawEyes();

    lcd.setCursor(0,1);
    lcd.print("Temp:");
    lcd.print(temperature);
    lcd.print((char)223);
    lcd.print("C ");

    if(digitalRead(MENU_BTN)==LOW)
    {
      beep();
      delay(300);
      showMenu();
      return;
    }

    delay(500);
  }
}

// -------------------Timer--------------
void showTimer()
{
  lcd.clear();

  timerRunning = true;
  timerStart = millis();

  while(timerRunning)
  {
    unsigned long remain =
      timerDuration - (millis() - timerStart);

    int sec = remain/1000;

    drawEyes();

    lcd.setCursor(0,1);
    lcd.print("Timer:");
    lcd.print(sec);
    lcd.print("s ");

    if(remain<=0)
    {
      timerRunning=false;

      lcd.clear();

      lcd.print("TIME UP!");

      for(int i=0;i<5;i++)
      {
        beep();
        delay(300);
      }

      delay(1000);

      showMenu();

      return;
    }

    if(digitalRead(MENU_BTN)==LOW)
    {
      timerRunning=false;
      beep();
      delay(300);
      showMenu();
      return;
    }
  }
}

//----------------Stop Watch-----------

void showStopwatch()
{
  while (digitalRead(SELECT_BTN) == LOW);
  delay(200);

  lcd.clear();

  stopwatchRunning = true;
  stopwatchStart = millis();

  while (stopwatchRunning)
  {
    unsigned long seconds = (millis() - stopwatchStart) / 1000;

    drawEyes();

    lcd.setCursor(0,1);
    lcd.print("SW:");
    lcd.print(seconds);
    lcd.print(" sec   ");

    // Stop stopwatch
    if (digitalRead(SELECT_BTN) == LOW)
    {
      while (digitalRead(SELECT_BTN) == LOW);

      beep();

      stopwatchRunning = false;

      lcd.clear();
      lcd.print("Stopped");

      lcd.setCursor(0,1);
      lcd.print(seconds);
      lcd.print(" sec");

      delay(1500);

      showMenu();
      return;
    }

    // Return to menu
    if (digitalRead(MENU_BTN) == LOW)
    {
      while (digitalRead(MENU_BTN) == LOW);

      beep();

      stopwatchRunning = false;

      showMenu();
      return;
    }

    delay(100);
  }
}
//-------------second part--------
//----------Third part-------------

//------------reminders--------

void reminder(String title)
{
  lcd.clear();

  drawEyes();

  lcd.setCursor(0,1);
  lcd.print(title);

  for(int i=0;i<3;i++)
  {
    beep();
    delay(200);
  }

  delay(1500);

  showMenu();
}

//-----------------Motivation Quote Function-------

// void reminder(String title)
//   {
//   lcd.clear();

//   drawEyes();

//   lcd.setCursor(0,1);
//   lcd.print(title);

//   for(int i=0;i<3;i++)
//   {
//     beep();
//     delay(200);
//   }

//   delay(1500);

//   showMenu();
//   }
void showQuote()
{
  lcd.clear();

  drawEyes();

  int q=random(totalQuotes);

  lcd.setCursor(0,1);
  lcd.print(quotes[q]);

  delay(2500);

  showMenu();
}

//--------------About section-------

void showAbout()
{
  String msg = " Hello, friend! I am Smart Student Assistant Robot. I assist you with timers, temperature monitoring, homework reminders, motivational quotes, and many smart features to support your daily routine. ";

  while (true)
  {
    for (int i = 0; i < msg.length() - 15; i++)
    {
      lcd.clear();

      lcd.setCursor(0,0);
      drawEyes();

      lcd.setCursor(0,1);
      lcd.print(msg.substring(i, i + 16));

      delay(300);

      if(digitalRead(MENU_BTN)==LOW)
      {
        beep();
        delay(300);
        showMenu();
        return;
      }
    }
  }
}


//-----------------Third part--------------
void setup()
{
  pinMode(MENU_BTN,INPUT_PULLUP);
  pinMode(UP_BTN,INPUT_PULLUP);
  pinMode(DOWN_BTN,INPUT_PULLUP);
  pinMode(SELECT_BTN,INPUT_PULLUP);

  pinMode(BUZZER,OUTPUT);
  pinMode(FLASHLIGHT,OUTPUT);

  digitalWrite(FLASHLIGHT,LOW);

  lcd.init();
  lcd.backlight();

  lcd.createChar(0,eyeOpen);
  lcd.createChar(1,eyeClosed);

  lcd.clear();

  lcd.setCursor(2,0);
  lcd.print("Hii, Lokesh!!");

  lcd.setCursor(2,1);
  lcd.print("How are you?");

  beep();

  delay(5000);

  showMenu();
}
//--------third part start-------------
//-----------check reminders-----------

void checkReminders()
{
  if(!reminderEnable)
    return;

  if(millis()-waterTimer>WATER_INTERVAL)
  {
    waterTimer=millis();
    reminder("Drink Water!");
  }

  if(millis()-stretchTimer>STRETCH_INTERVAL)
  {
    stretchTimer=millis();
    reminder("Stretch Time");
  }

  if(millis()-homeworkTimer>HOMEWORK_INTERVAL)
  {
    homeworkTimer=millis();
    reminder("Homework!");
  }

  if(millis()-sleepTimer>SLEEP_INTERVAL)
  {
    sleepTimer=millis();
    reminder("Sleep Time");
  }
}

//-------------third part end-----------
void loop()
{
  checkReminders();
  //Eye Blink every 3 seconds

  if(millis()-previousBlink>3000)
  {
    blink=true;
    drawEyes();
    delay(150);

    blink=false;
    drawEyes();

    previousBlink=millis();
  }

  //MENU BUTTON

  if(digitalRead(MENU_BTN)==LOW)
  {
    beep();
    showMenu();
    delay(250);
  }

  //UP

  if(digitalRead(UP_BTN)==LOW)
  {
    beep();

    menuIndex--;

    if(menuIndex<0)
      menuIndex=totalMenus-1;

    showMenu();

    delay(250);
  }

  //DOWN

  if(digitalRead(DOWN_BTN)==LOW)
  {
    beep();

    menuIndex++;

    if(menuIndex>=totalMenus)
      menuIndex=0;

    showMenu();

    delay(250);
  }

  //SELECT

  if(digitalRead(SELECT_BTN)==LOW)
  {
  beep();

  switch(menuIndex)
    {
    case 0:
      showTemperature();
      break;

    case 1:
      showTimer();
      break;

    case 2:
      showStopwatch();
      break;

    case 4:
      showQuote();
    break;

    case 6:
      digitalWrite(FLASHLIGHT,!digitalRead(FLASHLIGHT));
      break;

    case 7:
      showAbout();
    break;

    default:
      lcd.clear();

      drawEyes();

      lcd.setCursor(0,1);

      lcd.print(menus[menuIndex]);

      delay(1000);

      showMenu();
      break;
    }

  delay(200);
  }

}
