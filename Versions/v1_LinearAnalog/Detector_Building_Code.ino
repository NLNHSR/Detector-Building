//Below is code for standard voltage to temp conversion:
#include <LiquidCrystal.h> 
#include <math.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  //Sets up LCD Screen
int sensorPin = 5;  //Analog pin(On Arduino) that the TMP36(Thermistor) is connected to
                    //Resolution is 10 mV / degree centigrade with 500 mv offset to allow for negative temps 
void setup()    //Runs once when Arduino is turned on
{
  Serial.begin(9600); //Start serial connection with computer(view results in serial monitor) 
  lcd.begin(16, 2);   //Initializes dimensions(W x H) of LCD screen
  pinMode(11, OUTPUT);  //Blue LED
  pinMode(12, OUTPUT);  //Green LED
  pinMode(13, OUTPUT);  //Red LED
  float dtemp = 0;
  
}
void loop() //Code that is looping during the execution of the program
{
 
  int reading = analogRead(sensorPin);  //Gets voltage reading from thermistor 
 //Converts reading to voltage
 float voltage = reading * 5.0;
 voltage /= 1024.0;  
 Serial.print(voltage); Serial.println(" volts"); // print out voltage in serial monitor
 float R = voltage/25000

  ;int temperatureC = 100
 ;Serial.print(temperatureC); Serial.println(" degrees C");  //print out temp in serial monitor

  //Output voltage and temp to LCD screen 
 lcd.setCursor(0,0);
  
 lcd.print("Temp(C):   "); lcd.print(temperatureC)
 ;lcd.setCursor(0,1);
 lcd.print("Volt(V):    "); lcd.print(voltage)
 //Set of "if tests" to determine temp range and turn on corresponding LED's
 ;if (temperatureC < 25) {
   ;digitalWrite(11, HIGH);
   ;digitalWrite(12, LOW);
   ;digitalWrite(13, LOW);
 }
  else if (temperatureC >=25 and temperatureC < 50) {
    ;digitalWrite(11, LOW);
    ;digitalWrite(12, HIGH);
    ;digitalWrite(13, LOW);
 }
  else if (temperatureC >= 50) {
    ;digitalWrite(11, LOW);
    ;digitalWrite(12, LOW);
    ;digitalWrite(13, HIGH);
 }
 ;delay(500) //Waits a second to repeat code
;}  
