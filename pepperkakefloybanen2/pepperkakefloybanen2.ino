/*
 fløyen opp/ned med parallax servo/dc motor drivere på pin 2 og 3, sammen med potmeter på A4 og A5
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
                // twelve servo objects can be created on most boards


// Editable parameters
// How many seconds should it go downwards (upwards movement controlled by sensor)
int goDownTime=11*60;

// How long break at the end station (seconds)?
const long motorstoptime = 20*60; // hvor lang pause før retning snus



int led = 13;
int motorruntime = 0; // hvor lenge motor skal gå i en retning
boolean direction = 0; // 0 = opp, 1 = ned

int topSensorBM=6;
int topSensorRH=5;

int downSpeedRH=98;
int downSpeedBM=99;

int upSpeedRH=84;
int upSpeedBM=84;

boolean topReachedRH=0; 
boolean topReachedBM=0;


boolean topStatusRH=0;
boolean topStatusBM=0;

int turn=0;

//Current speed
int rh=90;
int bm=90;

void setup()
{
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(3);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  myservo.write(upSpeedRH);
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


void printStatus(){
  Serial.print("loop: ");
  Serial.print(motorruntime);
  Serial.print(". ");
  if(topStatusBM) Serial.print("Blaamann");
  if(topStatusBM && topStatusRH) Serial.print(" og ");
  if(topStatusBM) Serial.print("Rodhette");
  if(topStatusBM || topStatusRH) Serial.print(" er pa toppen, ");
  if(turn) Serial.print("Rodhette kjorer");
  else Serial.print("Blaamann kjorer");

  Serial.print("; Motorer RH:");
  Serial.print(rh);
  
  Serial.print(" BM:");
  Serial.print(bm);
  Serial.print("\r");
}

void loop()
{
  topStatusBM=digitalRead(topSensorBM);
  topStatusRH=digitalRead(topSensorRH);

  printStatus();
  if(sync()){
  int runtimeval = goDownTime;//map(sensorValueA4, 0, 1023, 10, 40);
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  rh=90;
  bm=90;
  if (direction)
  {
    if(!topStatusRH && turn == 1)rh=upSpeedRH;
    if(!topStatusBM && turn == 0)bm=upSpeedBM;
  } else
  { //Blamann goes DOWN
    if(motorruntime <= runtimeval && turn == 1)rh=downSpeedRH;
    if(motorruntime <= runtimeval && turn == 0)bm=downSpeedBM;
    
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
    delay(motorstoptime*1000); // Pause hvis oppe eller nede
    // skifte retning
    if (direction) {
      direction = 0;
      if(turn == 0)turn=1;
      else turn=0;
    } else { direction = 1; }
  }
  }
}
