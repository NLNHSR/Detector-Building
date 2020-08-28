#include <LiquidCrystal.h> 
#include <math.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  //Sets up LCD Screen 
// which analog pin to connect
#define THERMISTORPIN A5              
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 10
// the value of the 'other' resistor
#define SERIESRESISTOR 9930    
float logR2;
int samples[NUMSAMPLES];

// kalman variables
float varVolt = 1.12184278324081E-05;  // variance determined using excel and reading samples of raw sensor data
float varProcess = 1e-8;
float Pc = 0.0;
float G = 0.0;
float P = 1.0;
float Xp = 0.0;
float Zp = 0.0;
float Xe = 0.0;


void setup(void) {
  Serial.begin(9600);
  analogReference(AR_DEFAULT);
  lcd.begin(16, 2);   //Initializes dimensions(W x H) of LCD screen
  pinMode(11, OUTPUT);  //Blue LED
  pinMode(12, OUTPUT);  //Green LED
  pinMode(13, OUTPUT);  //Red LED
}
 
void loop(void) {
  
  uint8_t i;
  float average;
  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
  
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;

  // kalman process
  Pc = P + varProcess;
  G = Pc/(Pc + varVolt);    // kalman gain
  P = (1-G)*Pc;
  Xp = Xe;
  Zp = Xp;
  Xe = G*(average-Zp)+Xp;   // the kalman estimate of the sensor voltage
  
  Serial.print("Average analog reading "); 
  Serial.println(Xe);
  
  // convert the value to resistance
  Xe = 1023 / Xe - 1;
  Xe = SERIESRESISTOR / Xe;
  Serial.print("Thermistor resistance "); 
  Serial.println(Xe);
  
  double steinhart;
  //From Online Chart:
  float A = .001126007582;
  float B = .000234609648;
  float C = .00000008600840316;
  //Experimental Calibration Test Data:
  /*
  float A = .001138291321;
  float B = .0002302608497;
  float C = .0000001206148881;
  */
  logR2 = log(Xe);
  steinhart = (1.0 / (A + B * logR2 + C * logR2 * logR2 * logR2));  // Steinhart and Hart Equation. T  = 1 / {A + B[ln(R)] + C[ln(R)]^3}
  steinhart = steinhart -  273.15;                         // convert to C
  int reading = analogRead(THERMISTORPIN);  //Gets voltage reading from thermistor 
  //Converts reading to voltage
  float voltage = reading * 3.3;
  voltage /= 1023.0;  
  
  Serial.print("Temperature "); 
  Serial.print(steinhart);
  Serial.println(" *C");
  //Output voltage and temp to LCD screen 
  lcd.setCursor(0,0);
  
  lcd.print("Temp(C):   "); lcd.print(steinhart)
  ;lcd.setCursor(0,1);
  lcd.print("Volt(V):    "); lcd.print(voltage)
  //Set of "if tests" to determine temp range and turn on corresponding LED's
  ;if (steinhart < 25) {
   ;digitalWrite(11, HIGH);
   ;digitalWrite(12, LOW);
   ;digitalWrite(13, LOW);
  }
  else if (steinhart >=25 and steinhart < 50) {
    ;digitalWrite(11, LOW);
    ;digitalWrite(12, HIGH);
    ;digitalWrite(13, LOW);
  }
  else if (steinhart >= 50) {
    ;digitalWrite(11, LOW);
    ;digitalWrite(12, LOW);
    ;digitalWrite(13, HIGH);
  }
  delay(1000);
}
