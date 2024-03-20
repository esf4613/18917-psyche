#include <Servo.h>

// Servo variables
Servo servo;
const byte servoPin = A3; // coolant enable
byte pos = 0;
const int servoClosed = 5;
const int servoOpen = 60;

// Stepper variables
const int enPin=8;
const int stepXPin = 2; //X.STEP
const int dirXPin = 5; // X.DIR
const int stepYPin = 3; //Y.STEP
const int dirYPin = 6; // Y.DIR
const int stepZPin = 4; //Z.STEP
const int dirZPin = 7; // Z.DIR

const int h_steps=1000; // Horizontal stepper steps (200 per revolution)
const int v_steps=1000; // Horizontal stepper steps (200 per revolution)
const int c_steps=105; // Horizontal stepper steps (200 per revolution)
int pulseWidthMicros = 100; 	// microseconds
int millisBtwnSteps = 1000;

int motorMoveTime = 10000; // 10 Seconds

void setup() {
  // CNC shield setup
 	Serial.begin(9600);
 	pinMode(enPin, OUTPUT);
 	digitalWrite(enPin, LOW);
 	pinMode(stepXPin, OUTPUT);
 	pinMode(dirXPin, OUTPUT);
  pinMode(stepYPin, OUTPUT);
 	pinMode(dirYPin, OUTPUT);
  pinMode(stepZPin, OUTPUT);
 	pinMode(dirZPin, OUTPUT);
 
  Serial.setTimeout(60000); // Timeout after 60 seconds
 	Serial.println(F("CNC Shield Initialized"));

  // Servo setup
  servo.attach(servoPin);
}

void loop() {
  Serial.println(F("Motor Control Menu:\n1 - Horizontal\n2 - Vertical\n3 - Claw\n4 - Collection Cylinder\nChoice: "));
  char motor = Serial.parseInt();
  int direction;

  if(motor == 1){ // Horizontal Motor
    Serial.println(F("Horizontal Motor selected."));
    Serial.println(F("Choose Direction:\n1 - Forward\n2 - Backward\nChoice: "));
    direction = Serial.parseInt();
    if (direction == 1){
      Serial.println(F("Forward selected. Driving..."));
      h_forward();
    }
    else if (direction == 2){
      Serial.println(F("Backward selected. Driving..."));
      h_backward();
    }
    else{
      Serial.println(F("Incorrect option entered. Please try again."));
    }
  }
  else if(motor == 2){ // Vertical Motor
    Serial.println(F("Vertical Motor selected."));
    Serial.println(F("Choose Direction:\n1 - Downward\n2 - Upward\n"));
    direction = Serial.parseInt();
    if (direction == 1){
      Serial.println(F("Downward selected. Driving..."));
      v_forward();
    }
    else if (direction == 2){
      Serial.println(F("Upward selected. Driving..."));
      v_backward();
    }
    else{
      Serial.println(F("Incorrect option entered. Please try again.\n"));
    }
  }
  else if(motor == 3){ // Claw
    Serial.println(F("Claw Motor selected."));
    Serial.println(F("Choose Direction:\n1 - Open\n2 - Closed\nChoice: "));
    direction = Serial.parseInt();
    if (direction == 1){
      Serial.println(F("Open selected. Driving..."));
      servo.write(servoOpen);
    }
    else if (direction == 2){
      Serial.println(F("Closed selected. Driving..."));
      servo.write(servoClosed);
    }
    else{
      Serial.println(F("Incorrect option entered. Please try again.\n"));
    }
  }
  else if(motor == 4){ // Collection Cylinder
    Serial.println(F("Collection Cylinder Motor selected."));
    Serial.println(F("Choose Direction:\n1 - Clockwise\n2 - Counterclockwise\nChoice: "));
    direction = Serial.parseInt();
    if (direction == 1){
      Serial.println(F("Clockwise selected. Driving..."));
      c_clockwise();
    }
    else if (direction == 2){
      Serial.println(F("Counterclockwise selected. Driving..."));
      c_counterclockwise();
    }
    else{
      Serial.println(F("Incorrect option entered. Please try again.\n"));
    }
  }
}

void h_forward(){
  Serial.println(F("Running clockwise"));
 	digitalWrite(dirXPin, HIGH); // Enables the motor to move in a particular direction
 	// Makes 200 pulses for making one full cycle rotation
 	for (int i = 0; i < h_steps; i++) {
 			digitalWrite(stepXPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepXPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}

void h_backward(){
  Serial.println(F("Running counter-clockwise"));
 	digitalWrite(dirXPin, LOW); //Changes the rotations direction
 	// Makes 400 pulses for making two full cycle rotation
 	for (int i = 0; i < h_steps; i++) {
 			digitalWrite(stepXPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepXPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}

void v_forward(){
  Serial.println(F("Running clockwise"));
 	digitalWrite(dirYPin, HIGH); // Enables the motor to move in a particular direction
 	// Makes 200 pulses for making one full cycle rotation
 	for (int i = 0; i < v_steps; i++) {
 			digitalWrite(stepYPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepYPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}

void v_backward(){
  Serial.println(F("Running counter-clockwise"));
 	digitalWrite(dirYPin, LOW); //Changes the rotations direction
 	// Makes 400 pulses for making two full cycle rotation
 	for (int i = 0; i < v_steps; i++) {
 			digitalWrite(stepYPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepYPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}

void c_clockwise(){
  Serial.println(F("Running clockwise"));
  digitalWrite(dirZPin, LOW); //Changes the rotations direction
 	// Makes 400 pulses for making two full cycle rotation
 	for (int i = 0; i < c_steps; i++) {
 			digitalWrite(stepZPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepZPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}

void c_counterclockwise(){
  Serial.println(F("Running counter-clockwise"));
 	digitalWrite(dirZPin, HIGH); // Enables the motor to move in a particular direction
 	// Makes 200 pulses for making one full cycle rotation
 	for (int i = 0; i < c_steps; i++) {
 			digitalWrite(stepZPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepZPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}