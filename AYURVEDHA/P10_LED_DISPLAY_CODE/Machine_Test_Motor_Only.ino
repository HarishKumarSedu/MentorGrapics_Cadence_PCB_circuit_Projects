//program is working on invert logic.
#include<LiquidCrystal.h>
LiquidCrystal lcd(34,35,30,31,32,33);
#define flowmeter 2
#define start_button A2
#define NTC A1
#define w_clk A0
#define w_clk_relay 3
#define heater_relay 4
#define motor 5
#define solenoid 6

#define solenoid2 7
#define LED1 8
#define LED2 9

#include<math.h>
double start, new_time, start2, new_time2, temperature,array_temp[101],total_temp, time_prog, time_prog2;
volatile int  flow_frequency;
int toggle = 10;
int flow, a, b = 0, count = 0,c;
float required_time1 = 4, required_time2 = 0, required_time3 = 9.6;
bool hflag, wflag = true, testLED, progLED;

void setup()
{
  pinMode(28,OUTPUT);
  digitalWrite(28,HIGH);
  lcd.begin(20,4);
  //lcd.setCursor(0,1);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("BRAHM WORKS");
  delay(2000);
  lcd.clear(); 
  //lcd.setCursor(0,2);
  lcd.print("BSH DISHWASHER");
  delay(2000);
  lcd.clear();
  Serial.begin(9600);
  pinMode(flowmeter, INPUT);
 // digitalWrite(flowmeter, HIGH); //Internal Pull-up
  pinMode(w_clk, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(start_button, INPUT_PULLUP);
  pinMode(toggle, INPUT_PULLUP);
  pinMode(NTC, INPUT);

  pinMode(w_clk_relay, OUTPUT);
  pinMode(heater_relay, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(solenoid, OUTPUT);
  flow_frequency = 0;
  attachInterrupt(0, flow, RISING);
  sei();
  digitalWrite(w_clk_relay, HIGH); //invert logic
  digitalWrite(heater_relay, HIGH); //invert logic
  digitalWrite(motor, HIGH); //invert logic
  digitalWrite(solenoid, HIGH); //invert logic
  Serial.println("HOME");
  

}

void loop() {
  //put your main code here, to run repeatedly:
  //time_prog=2;
  lcd.setCursor(0,2);
  lcd.print("Motor test only");
  if (digitalRead(toggle))
  {
    testLED = true;
    progLED = false;
    time_prog = 2;
    time_prog2 = 10;
    Serial.println("Program Mode: TEST");
    lcd.setCursor(0,4);
    lcd.print("Program Mode: TEST");
  }
  else
  {
    time_prog = 40;
    time_prog2 = 10;
    testLED = false;
    progLED = true;
    Serial.println("Program Mode: ACTUAL");
    lcd.setCursor(0,4);
    lcd.print("Program Mode: ACTUAL");
  }
  
  digitalWrite(LED1, testLED);
  digitalWrite(LED2, progLED);
  b = analogRead(start_button);

  if ( b > 900)
  {
    Serial.println("Start button ON");
    lcd.setCursor(0,4);
    lcd.clear();
    lcd.print("Start Button ON");
    washing_cycle();
    digitalWrite(motor, LOW);  //invert logic
    delay(300000); // run for 5 mins
    digitalWrite(motor, HIGH);
    /*
    drain(); //Fail Safe - Pre drain
    //washing_cycle();
    //drain();
    //rinse1();
    //drain();
    //washing_cycle();
    //drain();
    drying();
    drain();
    */
  }
  
}
void washing_cycle()
{
  reset_w_clk();
  //lcd.clear();
  Serial.println("Setting water clock to wash position");
  //lcd.print("Wtr_CLK reset");
  digitalWrite(w_clk_relay, LOW);//invert logic
  delay(required_time1 * 1000);
  digitalWrite(w_clk_relay, HIGH);//invert logic
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Wtr_CLK = wash");
  Serial.println("Water clock set to wash position");
  delay(1000);
  /*
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Flow of Water");
  delay(1000);
  Serial.println("Starting flow of water");
  flow_frequency = 0;
  //flowcontrol();
  flow_frequency = 0;
  lcd.setCursor(0,1);
  lcd.print("Starting Heater");
  Serial.println("Starting heater");
  digitalWrite(heater_relay, LOW);//invert logic
  hflag = false;
  */
  lcd.setCursor(0,2);
  lcd.print("Motor For Wash");
  Serial.println("Starting Motor for washing");
  /*
  digitalWrite(motor, LOW);//invert logic
  start = millis();

  do
  {
    tempcontrol();
    new_time = millis();
    lcd.setCursor(0,3);
    lcd.print("time:");
    lcd.setCursor(10,3);
    lcd.print(int((new_time - start)/1000)/60);
    lcd.setCursor(12,3);
    lcd.print(":");
    lcd.setCursor(13,3);
    lcd.print(int((new_time - start)/1000)%60);
    if((int((new_time - start)/1000)%60)==0)
    {
    lcd.setCursor(0,3);
    lcd.print("                    ");
    lcd.setCursor(0,3);
    lcd.print("time:");
    lcd.setCursor(13,3);
    lcd.print(int((new_time - start)/1000)%60);
    lcd.setCursor(13,3);
    lcd.print("0");
    }
  } while (new_time - start < (time_prog * 60 * 1000));
  
  
  digitalWrite(heater_relay, HIGH);//switching off for failsafe //invert logic
  digitalWrite(motor, HIGH); //invert logic
  Serial.println("Washing Cycle Complete");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("Wash Complete");
  */
}

void rinse1()
{
  reset_w_clk();
  //lcd.clear();
  Serial.println("Setting water clock to wash position");
  //lcd.print("Wtr_CLK reset");
  digitalWrite(w_clk_relay, LOW);//invert logic
  delay(required_time1 * 1000);
  digitalWrite(w_clk_relay, HIGH);//invert logic
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Wtr_CLK = rinse");
  Serial.println("Water clock set to rinse position");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Flow of Water");
  delay(1000);
  Serial.println("Starting flow of water");
  flow_frequency = 0;
  //flowcontrol();
  flow_frequency = 0;
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("Motor For Rinse");
  Serial.println("Starting Motor for rinsing");
  digitalWrite(motor, LOW);//invert logic
  start = millis();

  do
  {
    new_time = millis();
    lcd.setCursor(0,3);
    lcd.print("time:");
    lcd.setCursor(10,3);
    lcd.print(int((new_time - start)/1000)/60);
    lcd.setCursor(12,3);
    lcd.print(":");
    lcd.setCursor(13,3);
    lcd.print(int((new_time - start)/1000)%60);
    if((int((new_time - start)/1000)%60)==0)
    {
    lcd.setCursor(0,3);
    lcd.print("                    ");
    lcd.setCursor(0,3);
    lcd.print("time:");
    lcd.setCursor(13,3);
    lcd.print(int((new_time - start)/1000)%60);
    lcd.setCursor(13,3);
    lcd.print("0");
    }
  } while (new_time - start < (time_prog * 60 * 1000));
  
  
  digitalWrite(heater_relay, HIGH);//switching off for failsafe //invert logic
  digitalWrite(motor, HIGH); //invert logic
  Serial.println("Rinse Cycle Complete");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("Rinse Complete");
}


void drain()
{

  reset_w_clk();
  digitalWrite(w_clk_relay, LOW);//invert logic
  delay(required_time2 * 1000);
  digitalWrite(w_clk_relay, HIGH);//invert logic
  lcd.clear();
  lcd.print("Wtr_CLK = drain");
  delay(1000);
  Serial.println("Water clock set to drain position");
  delay(1000);
  Serial.println("Starting motor to drain water");
  lcd.clear();
  lcd.print("Motor = drain");
  start2 = millis();
  digitalWrite(motor, LOW);//invert logic
  do
  {
    new_time2 = millis();
    lcd.setCursor(0,3);
    lcd.print("time:");
    lcd.setCursor(10,3);
    lcd.print(int((new_time2 - start2)/1000)/60);
    lcd.setCursor(12,3);
    lcd.print(":");
    lcd.setCursor(13,3);
    lcd.print(int((new_time2 - start2)/1000)%60);
    if((int((new_time2 - start2)/1000)%60)==0)
    {
    lcd.setCursor(0,3);
    lcd.print("time:");
    lcd.setCursor(13,3);
    lcd.print(int((new_time2 - start2)/1000)%60);
    lcd.setCursor(13,3);
    lcd.print("0 ");
    }
  } while (new_time2 - start2 < 1200);
  digitalWrite(heater_relay, HIGH);//switching off for fail safe //invert logic
  digitalWrite(motor, HIGH);//invert logic
  //Serial.println("Drain cycle complete");
  lcd.clear();
  lcd.print("Drain done");
  delay(1000);
}


void drying()
{
  reset_w_clk();
  digitalWrite(w_clk_relay, LOW);//invert logic
  delay(required_time3 * 1000);
  digitalWrite(w_clk_relay, HIGH);//invert logic
  lcd.print("water clock set to drying position ");
  delay(1000);
  Serial.println("Water clock set to dry position");
  lcd.print("Starting flow of Water");
  Serial.println("Starting flow of water");
  flow_frequency = 0;
  flowcontrol();
  flow_frequency = 0;
  Serial.println("Starting Motor for drying");
  digitalWrite(motor, LOW);//invert logic
  start = millis();

  do
  {
    new_time = millis();
    lcd.setCursor(0,3);
    lcd.print("time:");
    lcd.setCursor(10,3);
    lcd.print(int((new_time - start)/1000)/60);
    lcd.setCursor(12,3);
    lcd.print(":");
    lcd.setCursor(13,3);
    lcd.print(int((new_time - start)/1000)%60);
    if((int((new_time - start)/1000)%60)==0)
    {
    lcd.setCursor(0,3);
    lcd.print("                    ");
    lcd.setCursor(0,3);
    lcd.print("time:");
    lcd.setCursor(13,3);
    lcd.print(int((new_time - start)/1000)%60);
    lcd.setCursor(13,3);
    lcd.print("0");
    }
  } while (new_time - start < time_prog2 * 60 * 1000);
  digitalWrite(heater_relay, HIGH);//switching off for failsafe or for demo. //invert logic
  digitalWrite(motor, HIGH);//invert logic
  lcd.print("Drying Cycle is completed ");
  Serial.println("Drying cycle complete");
}

void tempcontrol()
{
  total_temp = 0;
  c=0;
  //array_temp[0] = Thermistor(analogRead(NTC));
  for (int i = 1 ; i < 101; i++)
  {
    array_temp[0] = 25;
    array_temp[i] = Thermistor(analogRead(NTC));

    if (abs(array_temp[i] - array_temp[i - 1]) < (0.2 * array_temp[i - 1]))
    {
      total_temp = total_temp + array_temp[i];
      c++;
    }temperature = total_temp / c;
    

  }

  //temperature = Thermistor(analogRead(NTC));
  //Serial.println(temperature);
  lcd.setCursor(0,2);
  lcd.print("Temp:");
  lcd.setCursor(6,2);
  lcd.print(temperature);
  
  //delay(500);
  if (temperature > 85)
    hflag = true;

    
  if (!hflag)
  {
    digitalWrite(heater_relay, LOW);//invert logic
    Serial.println(temperature);
  }
  else
  {

    digitalWrite(heater_relay, HIGH); //invert logic
    hflag = true; // fail safe
  }
}

/*void flowcontrol()
  {
  digitalWrite(solenoid, LOW);//invert logic
  flow_frequency = 0;
  while (flow_frequency < 2115)
  {
    flow = digitalRead(flowmeter);
    if (flow)
    {
      flow_frequency++;
      Serial.println(flow_frequency);
    }
    digitalWrite(solenoid, LOW);//invert logic
  }
  digitalWrite(solenoid, HIGH);//invert logic

  }*/

void flowcontrol()
{
  wflag = true;
  do
  {
    digitalWrite(solenoid, LOW);//invert logic
    Serial.println(flow_frequency);
    if (flow_frequency > 1890)
    {
      Serial.println("flow complete");
      lcd.print("Flow Complete");
      wflag = false;
      digitalWrite(solenoid, HIGH); //invert logic
      flow_frequency = 0;
    }
  } while (wflag);

}

double Thermistor(int RawADC) {
  double Temp1, Resistance;
  Resistance = 10000 / ((1024.0 / RawADC) - 1);
  Temp1 = log(Resistance);
  Temp1 = 1 / (0.0009824098841 + (0.0002412847723 * Temp1) + (0.0000001270759467 * Temp1 * Temp1 * Temp1));
  Temp1 = Temp1 - 273.15;
  return Temp1;
}


void reset_w_clk()
{
  Serial.println("Reseting Water Clock");
  digitalWrite(w_clk_relay, LOW); //invert logic
  count = 0;
  do
  {
    a = analogRead(w_clk);
    Serial.println(a);
    delay(500);
    if (a < 200)
      count = 0;
    if (a > 200)
      count++;
    if (count == 3)
      break;
  } while (true);
  digitalWrite(w_clk_relay, HIGH); //invert logic
  Serial.println("Clock in reset position");
  lcd.clear();
  lcd.print("wtr_CLK = reset");

}
void flow_1()
{
  flow_frequency++;
}


