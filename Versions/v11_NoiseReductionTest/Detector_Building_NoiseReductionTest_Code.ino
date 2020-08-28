#define PIN A5

const int RunningAverageCount = 20;
float RunningAverageBuffer[RunningAverageCount];
int NextRunningAverage;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  analogReference(EXTERNAL);
}

void loop() {
  // put your main code here, to run repeatedly:
  float reading = analogRead(PIN);
  Serial.print(reading);
  //Serial.print('\n');
  Serial.print(" ");
  //Serial.print('\n');
  RunningAverageBuffer[NextRunningAverage++] = reading;
  if (NextRunningAverage >= RunningAverageCount){
    NextRunningAverage = 0; 
  }
  float RunningAverageTemperature = 0;
  for(int i=0; i< RunningAverageCount; ++i){
    RunningAverageTemperature += RunningAverageBuffer[i];
  }
  RunningAverageTemperature /= RunningAverageCount;
  float average = RunningAverageTemperature;

  Serial.print(average);
  Serial.print('\n');
}
