/*
 fløyen opp/ned med parallax servo/dc motor drivere på pin 2 og 3, sammen med potmeter på A4 og A5
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
                // twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int led = 13;
int motorruntime = 0; // hvor lenge motor skal gå i en retning
const int motorstoptime = 5000; // hvor lang pause før retning snus
int motorspeed = 0;
boolean direction = 0; // 0 = opp, 1 = ned

int upArr[] = {79, 80, 81, 82, 83, 84, 85};
//int upArr[] = {60, 65, 70, 75, 80, 85, 90};
int downArr[] = {104, 103, 102, 101, 100, 99, 98};
//int downArr[] = {100, 105, 110, 115, 120, 125, 130};

void setup()
{
  //delay(10000);
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(3);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  myservo.write(90);
  myservo2.write(90);
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
  } else
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
