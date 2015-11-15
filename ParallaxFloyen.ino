/* 
 fløyen opp/ned med parallax servo/dc motor drivere på pin 2 og 3, sammen med potmeter på A4 og A5
*/ 

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
Servo myservo2;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
 
const byte EndSensorA = 5;
const byte EndSensorB = 6;
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

const int upMotor = 85;
const int downMotor = 98;
const int stopMotor = 90;

void setup() 
{ 
  //delay(10000);
  myservo.attach(2);  // attaches the servo on pin 2 to the servo object 
  myservo2.attach(3);  // attaches the servo on pin 3 to the servo object 
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(EndSensorA, INPUT);
  pinMode(EndSensorB, INPUT);
  myservo.write(stopMotor);
  myservo2.write(stopMotor); //stop motors
  
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
        myservo2.write(upMotor);
        delay(2000);
        
        //when 2 seconds has gone, stop motor 1, but keep it going for motor 2 for the next
        //40 seconds
        myservo.write(stopMotor);
        myservo2.write(upMotor);
        delay(40000);
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
  delay(1000); //if using sensors to reset it each time it goes up, then this cant //be here..
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
      direction = false; 
    } else { direction = true; }
  }
} 
