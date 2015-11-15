#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
Servo myservo2;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
 
byte EndSensorA = 10;
byte EndSensorB = 11;
boolean EndSensorATrigger = false;
boolean EndSensorBTrigger = false;


int pos = 0;    // variable to store the servo position 
int led = 13;
int motorruntime = 0; // hvor lenge motor skal gå i en retning 
const int motorstoptime = 5000; // hvor lang pause før retning snus
int motorspeed = 0;
boolean direction = 0; // 0 = opp, 1 = ned

int upArr[] = {79, 80, 81, 82, 83, 84, 85};
int downArr[] = {104, 103, 102, 101, 100, 99, 98};

void setup() 
{ 
  //delay(10000);
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object 
  myservo2.attach(3);  // attaches the servo on pin 9 to the servo object 
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  myservo.write(90);
  myservo2.write(90); //stop motors
  
  
  //init both motors
   //both motors need to go into the up direction to hit the switches
  myservo.write(0);
  myservo2.write(0);
  
  while true
  {
      if digitalRead(EndSensorA)
      {
         myservo.write(90);
         EndSensorATrigger = true;
      }
      if digitalRead(EndSensorB)
      {
         myservo2.write(90);
         EndSensorBTrigger = true;
      }
      
      if (EndSensorATrigger && EndSensorBTrigger)
      {
        
        //when motors hit the sensors, go into the reverse direction for 2 seconds (same direction)
        myservo.write(180);
        myservo2.write(0);
        delay(2000);
        
        //when 2 seconds has gone, stop motor 1, but keep it going for motor 2 for the next
        //40 seconds
        myservo.write(90);
        myservo2.write(0);
        delay(40000);
        //once motor 2 has gone 40 seconds, stop motor 2
        myservo2.write(90);
        break; 
      }
      // wait until both triggers have been 
  }
  //motors are now initialised
  
} 
 
void loop() 
{ 
  int sensorValueA4 = analogRead(A4);
  int sensorValueA5 = analogRead(A5);
  int runtimeval = map(sensorValueA4, 0, 1023, 10, 40);
  int speedval = map(sensorValueA5, 0, 1023, 0, 6); // OBSOBS - størrelse på upArr/downArr må være lik
  
  Serial.print(sensorValueA4);
  Serial.print(" -> ");
  Serial.print(runtimeval);
  Serial.print(" - ");
  Serial.print(sensorValueA5);
  Serial.print(" -> ");
  Serial.println(speedval);

  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)

  if (direction) 
  {
    Serial.print(motorruntime);
    Serial.print(" -> ");
    Serial.print(upArr[speedval]);
    Serial.println(" - up");
    myservo.write(upArr[speedval]);
    myservo2.write(upArr[speedval]);
  } 
  else 
  {
    Serial.print(motorruntime);
    Serial.print(" -> ");
    Serial.print(downArr[speedval]);
    Serial.println(" - down");
    myservo.write(downArr[speedval]);
    myservo2.write(downArr[speedval]);
  }  
 
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  
  
  delay(1000); 
  motorruntime += 1;
  if (motorruntime > runtimeval) {
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
