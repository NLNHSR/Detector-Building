#include <LiquidCrystal.h> 
#include <math.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  //Sets up LCD Screen 
// which analog pin to connect
#define THERMISTORPIN A5         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 9999      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3435
// the value of the 'other' resistor
#define SERIESRESISTOR 9930    
 
int samples[NUMSAMPLES];
 
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
 
  Serial.print("Average analog reading "); 
  Serial.println(average);
  
  // convert the value to resistance
  average = (1023 / average - 1)*1.51515151515151515;
  average = SERIESRESISTOR / average;
  Serial.print("Thermistor resistance "); 
  Serial.println(average);
  
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
  
  int reading = analogRead(THERMISTORPIN);  //Gets voltage reading from thermistor 
  //Converts reading to voltage
  float voltage = reading * 3.3;
  voltage /= 1024.0;  
  
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
