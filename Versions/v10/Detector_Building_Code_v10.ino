#include <math.h>
#define THERMISTORPIN 13 //Which analog pin to connect
#define SERIESRESISTOR 9850 //Value of the 'other' resistor(voltage divider)

float series;
float logR2;

//Kalman filter variables
float varRes = 1.12184278324081E-05;  // variance determined using excel and reading samples of raw sensor data
float varProcess = 1e-8;
float Pc = 0.0;
float G = 0.0;
float P = 1.0;
float Xp = 0.0;
float Zp = 0.0;
float Xe = 0.0;

void setup()
{                
  Serial.begin(9600);
  analogReadResolution(13);
  pinMode(1, OUTPUT);  //Blue LED
  pinMode(3, OUTPUT);  //Green LED
  pinMode(5, OUTPUT);  //Red LED

}

void loop()                     
{
  int reading = analogRead(THERMISTORPIN);

  //Converts reading to voltage
  float voltage = reading * 3.3;
  voltage /= 8192.0; 

  //Converts reading to resistance
  float resistance = (1/((3.3/voltage)-1))*SERIESRESISTOR;

  
  //Kalman process
  Pc = P + varProcess;
  G = Pc/(Pc + varRes);    // kalman gain
  P = (1-G)*Pc;
  Xp = Xe;
  Zp = Xp;
  Xe = G*(resistance-Zp)+Xp;   // the kalman estimate of the sensor voltage  
  
  double steinhart;
  //From Online Chart:
  float A = .00132261287;
  float B = .0001981565484;
  float C = .0000002743119237;
  
  //Converts resistance to temperature using Steinhart and Hart Equation (T = 1 / {A + B[ln(R)] + C[ln(R)]^3}) 
  logR2 = log(Xe);
  steinhart = (1.0 / (A + B * logR2 + C * logR2 * logR2 * logR2)); 
  
  //Converts temperature from Kelving to Celsius  
  steinhart = steinhart -  273.15;

  //Output resistance and temperature to Serial monitor
  /*Serial.println("Resistance");
  Serial.println(resistance);
  Serial.println("Voltage");
  Serial.println(voltage);*/
  Serial.print("Thermistor resistance "); 
  Serial.println(resistance); Serial.print(",");
  Serial.print("Kalman resistance ");
  Serial.println(Xe);
 
  //Set of "if tests" to determine temp range and turn on corresponding LED's
  if (steinhart < 25) { //Lights up the Blue LED
   digitalWrite(1, HIGH);
   digitalWrite(3, LOW);
   digitalWrite(5, LOW);
  }
  else if (steinhart >=25 and steinhart < 50) { //Lights up the Green LED
    digitalWrite(1, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(5, LOW);
  }
  else if (steinhart >= 50) { //Lights up the Red LED
    digitalWrite(1, LOW);
    digitalWrite(3, LOW);
    digitalWrite(5, HIGH);
  }
  delay(10);
}
