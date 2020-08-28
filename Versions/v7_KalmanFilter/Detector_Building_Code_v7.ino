#include <LiquidCrystal.h> 
#include <math.h>
#define THERMISTORPIN A7 //Which analog pin to connect
#define SERIESRESISTOR 9850 //Value of the 'other' resistor(voltage divider)

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  //Sets up LCD Screen 

float logR2; //Steinhart and Hart variable

//Kalman filter variables
float varRes = 1.12184278324081E-05;  // variance determined using excel and reading samples of raw sensor data
float varProcess = 1e-8;
float Pc = 0.0;
float G = 0.0;
float P = 1.0;
float Xp = 0.0;
float Zp = 0.0;
float Xe = 0.0;

void setup(void) {
  Serial.begin(9600);
  analogReference(AR_DEFAULT);analogReadResolution(12); //Sets up analog input 
  lcd.begin(16, 2);   //Initializes dimensions(W x H) of LCD screen
  pinMode(11, OUTPUT);  //Blue LED
  pinMode(12, OUTPUT);  //Green LED
  pinMode(13, OUTPUT);  //Red LED
}
 
void loop(void) {
  
  int reading = analogRead(THERMISTORPIN);

  //Converts reading to voltage
  float voltage = reading * 3.3;
  voltage /= 4095.0; 
  
  //Converts reading to resistance
  //float resistance = 4095.0 / reading - 1;
  //resistance = SERIESRESISTOR / resistance;
  float resistance = (1/((3.3/voltage)-1))*SERIESRESISTOR;
  Serial.print("Thermistor resistance "); 
  Serial.println(resistance); Serial.print(",");

  //Kalman process
  Pc = P + varProcess;
  G = Pc/(Pc + varRes);    // kalman gain
  P = (1-G)*Pc;
  Xp = Xe;
  Zp = Xp;
  Xe = G*(resistance-Zp)+Xp;   // the kalman estimate of the sensor voltage  

  Serial.print("Kalman resistance ");
  Serial.println(Xe);
  
  double steinhart;
  //From Online Chart:
  //float A = .001126007582;
  //float B = .000234609648;
  //float C = .00000008600840316;
  //Experimental Calibration Test Data:
  
  float A = .00132261287;
  float B = .0001981565484;
  float C = .0000002743119237;
  
  //Converts resistance to temperature using Steinhart and Hart Equation (T = 1 / {A + B[ln(R)] + C[ln(R)]^3}) 
  logR2 = log(Xe);
  steinhart = (1.0 / (A + B * logR2 + C * logR2 * logR2 * logR2)); 
  //Converts temperature from Kelving to Celsius  
  steinhart = steinhart -  273.15;

  //Output voltage and temp to LCD screen 
  lcd.setCursor(0,0);
  
  lcd.print("Temp(C):   "); lcd.print(steinhart);
  lcd.setCursor(0,1);
  lcd.print("Volt(V):    "); lcd.print(voltage);
  
  //Set of "if tests" to determine temp range and turn on corresponding LED's
  if (steinhart < 25) { //Lights up the Blue LED
   digitalWrite(11, HIGH);
   digitalWrite(12, LOW);
   digitalWrite(13, LOW);
  }
  else if (steinhart >=25 and steinhart < 50) { //Lights up the Green LED
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
  }
  else if (steinhart >= 50) { //Lights up the Red LED
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
  }
  
  delay(20);
}
