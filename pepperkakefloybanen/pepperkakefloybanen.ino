/*
 fløyen opp/ned med parallax servo/dc motor drivere på pin 2 og 3, sammen med potmeter på A4 og A5
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
                // twelve servo objects can be created on most boards


// Editable parameters
// How many seconds should it go downwards (upwards movement controlled by sensor)
int goDownTime=600;

// How long break at the end station (milliseconds)?
const int motorstoptime = 10*60000; // hvor lang pause før retning snus



int pos = 0;    // variable to store the servo position
int led = 13;
int motorruntime = 0; // hvor lenge motor skal gå i en retning
int motorspeed = 0;
boolean direction = 0; // 0 = opp, 1 = ned

int upArr[] = {79, 80, 81, 82, 83, 84, 85};
//int upArr[] = {60, 65, 70, 75, 80, 85, 90};
int downArr[] = {104, 103, 102, 101, 100, 99, 98};
//int downArr[] = {100, 105, 110, 115, 120, 125, 130};

int topSensorBM=6;
int topSensorRH=5;

int downSpeedRH=99;
int downSpeedBM=84;

int upSpeedRH=83;
int upSpeedBM=98;

int topReachedRH=0;
int topReachedBM=0;


int topStatusRH=0;
int topStatusBM=0;

void setup()
{
  //delay(10000);
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(3);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  //myservo.write(90);
  //myservo2.write(90);
  myservo.write(downSpeedRH);
  myservo2.write(upSpeedBM); //Blaamann
}

boolean sync(){
  if(!topReachedBM)topReachedBM=topStatusBM;
  else{
    myservo2.write(90);
    //Serial.println("Blaamann er oppe!");
  }
  if(!topReachedRH)topReachedRH=topStatusRH;
  else{
    myservo.write(90);
    //Serial.println("Roedhette er oppe!");
  }
  if(topReachedBM && topReachedRH){
    return 1;
  }
  return 0;
}



void loop()
{
  topStatusBM=digitalRead(topSensorBM);
  topStatusRH=digitalRead(topSensorRH);
  Serial.print(topStatusBM);
  Serial.print(topStatusRH);
  Serial.print("\r");
  if(sync()){
  int runtimeval = goDownTime;//map(sensorValueA4, 0, 1023, 10, 40);
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)

  int rh=90;
  int bm=90;
  if (direction) // Blamann goes UP
  {
    if(motorruntime <= runtimeval)rh=downSpeedRH;
    if(!topStatusBM)bm=upSpeedBM;
  } else
  { //Blamann goes DOWN
    if(motorruntime <= runtimeval)bm=downSpeedBM;
    if(!topStatusRH)rh=upSpeedRH;
  }
  Serial.print(motorruntime);
  Serial.print(" Rodhette: ");
  Serial.print(rh);
  Serial.print(" Blaamann: ");
  Serial.print(bm);
  myservo.write(rh);
  myservo2.write(bm);


  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
  motorruntime += 1;
  if (motorruntime > runtimeval && (topStatusBM||topStatusRH)) {
    Serial.println("Delay and stop before reversing");
    motorruntime = 0;
    // Stopper servoer/motorer
    myservo.write(90);
    myservo2.write(90);
    delay(motorstoptime); // Pause hvis oppe eller nede
    // skifte retning
    if (direction) {
      direction = 0;
    } else { direction = 1; }
  }
  }
}
