#include <Time.h>
#include <TimeLib.h>

 /*This example illustrates two user funcions of LedP10 library
  *First function-showmsg_single_scroll is for scroling of single message on P10 panel with 4 arguments which are message, no. of times of message scrolling, speed and font.
  *for message, pointer to a character string can be passed. For continuously running the message, INF can be passed in second argument, spped can be in range of 0-30, 
   and font can be 0 or 1 as two type of fonts are currently supported for full screen messages. 
   
  * For second function-showmsg_double_scroll, there would be 7 arguments, 2 for messages, two for no. of times of message scrolling, two for speed, and last one for font. 
  * Specification being same as for single scrolling function, except only ont font id provided for double scrolling and it's value should be 0.
  * For connection diagram visit https://goo.gl/HiCJC1
  */
 
//#include <TimerOne.h>
#include"SPI.h"
#include <ledP10.h>
#include <Arial_black_16.h>
#include <double_font2.h>
#include <led_half_1.h>
#define fan_gnd 3    // define pin 3 for fan pin ground.

int number, x;          // number - incrementing count for fan.
int fanpin = 2;      // pin for MOSFET gate - Fan switch
int aqi;              // for final aqi value
int incoming[18];     // integer array for incoming ASCII values for AIRVEDA data
char pm25[5], pm10[5]; // to store pm2.5 and pm10 values from AIRVEDA
int timediff = 0, prevtime = 0;   // time values for fan.
bool Switchfan = true;

LedP10 myled;

void setup() 

{  
  
   Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(fanpin, OUTPUT);
  pinMode(fan_gnd, OUTPUT);
  prevtime = now();
  number = 0;         // Initiate fan switch counter to zero.
  Switchfan = true;   // Initially fan will be on
  digitalWrite(fan_gnd, LOW);

  myled.init(3,4,8,9,1);
  myled.showmsg_single_static("hi",0);
 delay(6000);
  
  
  Serial.println("hi ");
}

void loop() {
  myled.showmsg_single_static(pm25,0);
  delay(60000);
  
    
  if (Serial.available())
    for (int i = 0; i < 18; i++)
    {
      incoming[i] = Serial.read(); //GET array from airveda
      if (incoming[i] == 65)
        for (int j = 0; j < 3; j++)
          pm25[j] = incoming[i + j + 2]; //get pm2.5 array
    }

  for (int a = 0; a < 18; a++)
    if (incoming[a] == 66)
      for (int b = 0; b < 3; b++)
        pm10[b] = incoming[a + b + 2]; //get pm10 array

  Serial.println("Test");
  Serial.println(pm25);//display pm2.5
  Serial.println(pm10);//display pm10
  
//  myled.showmsg_single_static(num1,0);

  
  //delay(1000);

  aqi = getaqi(pm25, pm10);

  if(aqi == 1)
  {
      myled.showmsg_single_static(pm25,0);
      Serial.println("Displaying PM 2.5");
  }
  if(aqi == 0)
  {
      myled.showmsg_single_static(pm10,0);
      Serial.println("Displaying PM 10"); 
  }
  //lcd_print(aqi);
  /* for(int i=0;i<10;i++)
     {
      x=i*111;
     lcd_print(x);
     delay(1000);
     }
  */
  switch (number)
  {
    case 0:                              // First six hours fan is on
      timediff = now() - prevtime;
      //delay(1000);
      if (timediff >= 21600)
      {
        Switchfan = !Switchfan;
        number++;
        Serial.println("Fan switched off");
        prevtime = now();
      }

      break;

    case 1:                            // Next two hours fan is off

      timediff = now() - prevtime;
      //delay(1000);
      if (timediff >= 7200)
      {
        Switchfan = !Switchfan;
        number++;
        prevtime = now();
        Serial.println("Fan switched on");
      }
      break;

    case 2:                            // Next suz hours fan is on

      timediff = now() - prevtime;
      //delay(1000);
      if (timediff >= 21600)
      {
        Switchfan = !Switchfan;
        number++;
        prevtime = now();
        Serial.println("Fan switched off");
      }

      break;


    case 3:                            // Last ten hours fan is off

      timediff = now() - prevtime;
      //delay(1000);
      if (timediff >= 36000)
      {
        Switchfan = !Switchfan;
        number = 0;
        prevtime = now();
        Serial.println("Fan switched on");
      }

      break;

    default:                           // Default: Fan is on - Reset counter to 0;
      {
        Switchfan = true;
        number = 0;
      }
  }

  digitalWrite(fanpin, Switchfan);

}

void lcd_print(int num)
{
  int num1, num2, num3;

  num3 = num % 10;
  num /= 10;
  num2 = num % 10;
  num /= 10;
  num1 = num % 10;


  seg_print(A1, A1, A2, A3, num3);
  seg_print(8, 9, 10, 11, num2);
  seg_print(7, 6, 5, 4, num1);
}

void seg_print(int a, int b, int c, int d, int xnum)
{
  boolean bcd[4];
  for (int i = 3; i >= 0; i--)
  {
    bcd[i] = xnum % 2;
    xnum = xnum / 2;

  }
  digitalWrite(a, bcd[0]);
  digitalWrite(b, bcd[1]);
  digitalWrite(c, bcd[2]);
  digitalWrite(d, bcd[3]);

  //Serial.print(bcd[0]);
  //Serial.print(bcd[1]);
  //Serial.print(bcd[2]);
  //Serial.print(bcd[3]);
  //Serial.println('\n');

}

int getaqi( char x[], char y[])
{
  int a, b;           // To convert ASCII PM values to interger numbers
  int ddug, val = 0;  // Dust density in ugrams. Val is a return variable for AQI
  a = (x[2] - 48) + 10 * (x[1] - 48) + 100 * (x[0] - 48); // Converting PM 2.5 to Decimal
  b = (y[2] - 48) + 10 * (y[1] - 48) + 100 * (y[0] - 48); // Converting PM 10 to Decimal

  ddug = max(a, b);                                 // Dust density for AQI to be considered is the maximum of the two values.

Serial.println("pm 2.5 ");
Serial.print(a);
Serial.println("pm 10");
Serial.print(b);

  if(a>=b)
      return 1;

  else
      return 0;
 /* if (ddug <= 30)
    val = ddug * 50 / 30;

  else if (ddug > 30 && ddug <= 60)
    val = 50 + ((ddug - 30) * 50 / 30);

  else if (ddug > 60 && ddug <= 90)
    val = 100 + ((ddug - 60) * 100 / 30);

  else if (ddug > 90 && ddug <= 120)
    val = 200 + ((ddug - 90) * 100 / 30);

  else if (ddug > 120 && ddug <= 250)
    val = 300 + ((ddug - 120) * 100 / 130);

  else if (ddug > 250)
    val = 400 + ((ddug - 250) * 100 / 130);
  //Serial.println();
  //Serial.println();
  //delay(2000);
  Serial.println(val);

  return val;                                     //Returns AQI based on AIRVEDA mapping.
*/
  
}
