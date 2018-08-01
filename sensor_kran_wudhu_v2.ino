/*
 * NAMA ANGGOTA KELOMPOK:
 * 1. Ida Bagus Krishna Yoga Utama - 1506716983
 * 2. Ahmad Dhiyaaul Aulia - 1506717153
 * 3. Wilbert Tahir -1506747143
 * 4. Yanu Rahmaddianto - 1506717191
 * 
 * SD Card Module
 * pin 50 = MISO
 * pin 51 = MOSI
 * pin 52 = SCK
 * pin 53 = CS
 *
 * Flow Sensor
 * pin 2 = Signal
 *
 * Buzzer
 * pin 9 = Signal
 *
 * LED
 * pin 8 = Signal
 *
 * LCD
 * pin SDA = SDA
 * pin SCL = SCL
 *
 */

#include <SPI.h>
#include <SD.h>

#include <DS3231.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//*********LED STRIP*********//
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN          A0
#define NUMPIXELS    10

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 1;

const int max1 = 90;
const int max2 = 150;
const int max3 = 200;
const int max4 = 280;
const int max5 = 350;

//level1
const int level1R = 0;
const int level1G = 255;
const int level1B = 0;

//level2
const int level2R = 200;
const int level2G = 255;
const int level2B = 0;

//level3
const int level3R = 255;
const int level3G = 200;
const int level3B = 0;

//level4
const int level4R = 255;
const int level4G = 100;
const int level4B = 0;

//level5
const int level5R = 255;
const int level5G = 0;
const int level5B = 0;

//White
const int whiteR = 255;
const int whiteG = 255;
const int whiteB = 255;
//*********LED STRIP*********//

const int chipSelect = 53;
File      myFile;

DS3231 rtc(SDA, SCL);

volatile int  flow_frequency; // Measures flow sensor pulses
unsigned int  l_hour;         // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;

int flowhit, time1, time2, waktuPakai;

const int ledPin = 8;
const int buzzer = 9;

int count = 1;
int state = 0;

LiquidCrystal_I2C lcd(0x3F, 20, 4);

void flow()  // Interrupt function
{
   flow_frequency++;
}


void setup()
{
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH);   
   Serial.begin(9600);
   attachInterrupt(0, flow, RISING); 
   sei();                           
   currentTime = millis();
   cloopTime   = currentTime;

   pinMode(chipSelect, OUTPUT);

   while (!Serial)
   {
   }

   Serial.print("Initializing SD card...");

   if (!SD.begin(chipSelect))
   {
      Serial.println("Card failed, or not present");
      return;
   }
   Serial.println("card initialized.");

   pinMode(ledPin, OUTPUT);
   pinMode(buzzer, OUTPUT);

   rtc.begin();
   //rtc.setDate(25, 5, 2018);   //tanggal, bulan, tahun
   //rtc.setTime(20, 20, 00);     //jam, menit, detik

   lcd.begin();

   #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000)
    {
      clock_prescale_set(clock_div_1);
    }
   #endif
   
   pixels.begin();
}

void loop()
{
   myFile = SD.open("data.txt", FILE_WRITE);

   flowhit = hitungFlow();
   printToLCD();

   while (flowhit > 0.0)
   {
      state = 1;
      if (flowhit > max5)
      {
         pixels.setPixelColor(2, pixels.Color(level1R, level1G, level1B));
         pixels.show();
         pixels.setPixelColor(3, pixels.Color(level2R, level2G, level2B));
         pixels.show();
         pixels.setPixelColor(4, pixels.Color(level3R, level3G, level3B));
         pixels.show();
         pixels.setPixelColor(5, pixels.Color(level4R, level4G, level4B));
         pixels.show();
         pixels.setPixelColor(6, pixels.Color(level5R, level5G, level5B));
         pixels.show();   
         ledAndBuzzerOn();
      }
      else if ((flowhit > max4) && (flowhit < max5))
      {
         pixels.setPixelColor(2, pixels.Color(level1R, level1G, level1B));
         pixels.show();
         pixels.setPixelColor(3, pixels.Color(level2R, level2G, level2B));
         pixels.show();
         pixels.setPixelColor(4, pixels.Color(level3R, level3G, level3B));
         pixels.show();
         pixels.setPixelColor(5, pixels.Color(level4R, level4G, level4B));
         pixels.show();
         pixels.setPixelColor(6, pixels.Color(level5R, level5G, level5B));
         pixels.show();   
         ledAndBuzzerOn();   
      }
      else if ((flowhit > max3) && (flowhit < max4))
      {
         pixels.setPixelColor(2, pixels.Color(level1R, level1G, level1B));
         pixels.show();
         pixels.setPixelColor(3, pixels.Color(level2R, level2G, level2B));
         pixels.show();
         pixels.setPixelColor(4, pixels.Color(level3R, level3G, level3B));
         pixels.show();
         pixels.setPixelColor(5, pixels.Color(level4R, level4G, level4B));
         pixels.show();
         for (int i = 6; i < 7; i++)
         {
            pixels.setPixelColor(i, pixels.Color(whiteR, whiteG, whiteB));
            pixels.show();
            delay(delayval);
         }     
            ledAndBuzzerOff();     
      }
      else if ((flowhit > max2) && (flowhit < max3))
      {
         pixels.setPixelColor(2, pixels.Color(level1R, level1G, level1B));
         pixels.show();
         pixels.setPixelColor(3, pixels.Color(level2R, level2G, level2B));
         pixels.show();
         pixels.setPixelColor(4, pixels.Color(level3R, level3G, level3B));
         pixels.show();
         for (int i = 5; i < 7; i++)
         {
            pixels.setPixelColor(i, pixels.Color(whiteR, whiteG, whiteB));
            pixels.show();
            delay(delayval);
         }      
            ledAndBuzzerOff();    
      }
      else if ((flowhit > max1) && (flowhit < max2))
      {
         pixels.setPixelColor(2, pixels.Color(level1R, level1G, level1B));
         pixels.show();
         pixels.setPixelColor(3, pixels.Color(level2R, level2G, level2B));
         pixels.show();
         for (int i = 4; i < 7; i++)
         {
            pixels.setPixelColor(i, pixels.Color(whiteR, whiteG, whiteB));
            pixels.show();
            delay(delayval);
         }      
            ledAndBuzzerOff();    
      }
      else if ((flowhit > 0) && (flowhit < max1))
      {
         pixels.setPixelColor(2, pixels.Color(level1R, level1G, level1B));
         pixels.show();
         for (int i = 3; i < 7; i++)
         {
            pixels.setPixelColor(i, pixels.Color(whiteR, whiteG, whiteB));
            pixels.show();
            delay(delayval);
         }    
            ledAndBuzzerOff();      
      }
   
      printToSerial();
      printToSD();
      printToLCD();

      flowhit = hitungFlow();
      delay(500);
   }
   
   for (int i = 2; i < 7; i++)
   {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(delayval);
   }   

    if (state == 1)
    {
      count++;
      state = 0;
    }
    myFile.close();
    delay(500);
}

int hitungFlow()
{
   currentTime = millis();
   if (currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime;                     
      l_hour         = (flow_frequency * 60 / 4.5); 
      flow_frequency = 0;                       
   }
   return l_hour;
}


void printToSerial()
{
   Serial.print(rtc.getDateStr());
   Serial.print(" ");
   Serial.println(rtc.getTimeStr());
   Serial.print(" Value: ");
   Serial.println(flowhit);
}

void printToSD()
{
   myFile.print("Timestamp: ");
   myFile.print(rtc.getDateStr());
   myFile.print(" ");
   myFile.print(rtc.getTimeStr());
   myFile.print(" Value: ");
   myFile.print(flowhit);
   myFile.print(" Orang Ke- ");
   myFile.println(count);
    
}

void ledAndBuzzerOn()
{
   digitalWrite(ledPin, HIGH);
   tone(buzzer, 1000); //ganti nilai frekuensinya
}


void ledAndBuzzerOff()
{
   digitalWrite(ledPin, LOW);
   noTone(buzzer);
}


void printToLCD()
{
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print(rtc.getDateStr());
   lcd.print(" ");
   lcd.print(rtc.getTimeStr());
   lcd.setCursor(0, 2);
   lcd.print("Debit: ");
   lcd.print(flowhit);
   lcd.print(" L/jam");
   lcd.setCursor(0, 3);
   lcd.print("Orang Ke-");
   lcd.print(count);
}
