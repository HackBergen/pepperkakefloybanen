/* 
 fløyen opp/ned med parallax servo/dc motor drivere på pin 2 og 3, sammen med potmeter på A4 og A5
*/ 

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
Servo myservo2;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
 
const byte EndSensorA = 5; // venstre dvs rød lue
const byte EndSensorB = 6; // høyre dvs blå lue
boolean EndSensorATrigger = false;
boolean EndSensorBTrigger = false;
boolean EndSensorA_active = false;
boolean EndSensorB_active = false;


int pos = 0;    // variable to store the servo position 
const byte led = 13;
int motorruntime = 0; // hvor lenge motor skal gå i en retning 
const int motorstoptime = 5000; // hvor lang pause før retning snus
int motorspeed = 0;
boolean direction = false; // false = opp, true = ned

int upArr[] = {79, 80, 81, 82, 83, 84, 85};
int downArr[] = {104, 103, 102, 101, 100, 99, 98};

const int upMotor = 84; // 136s per finnelengde
const int downMotor = 105; // 
const int stopMotor = 90;
const int runtimeval = 1200; // * 1000ms per session

void setup() 
{ 
  //delay(10000);
  myservo.attach(2);  // Venstre vogn/motor dvs rød lue
  myservo2.attach(3);  // Høyre vogn/motor dvs blå lue
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(EndSensorA, INPUT);
  pinMode(EndSensorB, INPUT);

  //init both motors
  //both motors need to go into the up direction to hit the switches

  // Check if endsensors are already active before resetting motors
  if (digitalRead(EndSensorA)) 
  {
    EndSensorA_active = true;
  }
  if (digitalRead(EndSensorB)) 
  {
    EndSensorB_active = true;
  }
  
  if (!EndSensorA_active) 
  {
    myservo.write(upMotor);
  }
  if (!EndSensorB_active) {
    myservo2.write(upMotor);
  }
  
  while (true)
  {
      if (digitalRead(EndSensorA))
      {
         myservo.write(stopMotor);
         EndSensorATrigger = true;
      }
      
      if (digitalRead(EndSensorB))
      {
         myservo2.write(stopMotor);
         EndSensorBTrigger = true;
      }
      
      if (EndSensorATrigger && EndSensorBTrigger)
      {
        
        //when motors hit the sensors, go into the reverse direction for 2 seconds (same direction)
        myservo.write(downMotor);
        myservo2.write(downMotor);
        delay(2000);
        
        //when 2 seconds has gone, stop motor 1, but keep it going for motor 2 for the next
        //40 seconds
        myservo.write(stopMotor);
        myservo2.write(downMotor);
        delay(120000);
        //once motor 2 has gone 40 seconds, stop motor 2
        myservo2.write(stopMotor);
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

  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)

  if (direction) 
  {
    Serial.println("up");
    myservo.write(downMotor);
    myservo2.write(upMotor);
  } 
  else 
  {
    Serial.println("down");
    myservo.write(upMotor);
    myservo2.write(downMotor);
  }   
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000); //if using sensors to reset it each time it goes up, then this cant //be here..
  motorruntime += 1;
  if (motorruntime > runtimeval) {
    Serial.println("Delay and stop before reversing");
    motorruntime = 0;
    // Stopper servoer/motorer
    myservo.write(stopMotor);
    myservo2.write(stopMotor);
    delay(motorstoptime); // Pause hvis oppe eller nede
    // skifte retning
    if (direction) { 
      direction = false; 
    } else { direction = true; }
  }
} 
