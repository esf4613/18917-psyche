#include <Servo.h>

// Servo variables
Servo servo;
const byte servoPin = A3; // coolant enable
byte pos = 0;
const int servoClosed = 15; // Servo angle closed
const int servoOpen = 100; // Servo angle open

// Stepper variables
const int enPin=8;
const int stepXPin = 2; //X.STEP
const int dirXPin = 5; // X.DIR
const int stepYPin = 3; //Y.STEP
const int dirYPin = 6; // Y.DIR
const int stepZPin = 4; //Z.STEP
const int dirZPin = 7; // Z.DIR

// SparkFun VCNL4040 Proximity Sensors
const int proxMin = 0; // Nothing is detected
const int proxMax = 65535; // Max value for detecting an object
const int proxThresh = 200; // Value for sensor proximity

const int x_steps=7300; // Horizontal stepper steps (200 per revolution)
const int y_steps=10500; // Horizontal stepper steps (200 per revolution) [10,500 is highest value (max depth)]
const int z_steps=105; // Horizontal stepper steps (200 per revolution)
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
  Serial.println(F("Motor Control Menu:\n0 - Demo Program\n1 - Horizontal\n2 - Vertical\n3 - Claw\n4 - Collection Cylinder\nChoice: "));
  char motor = Serial.parseInt();
  int direction;
  
  if(motor == 0){
    Serial.println(F("Demo Program selected."));
    servo.write(servoOpen); // Open the claw
    h_v_abduction(); // Move claw over and down to the soil
    servo.write(servoClosed); // Close the claw
    h_v_adduction(); // Move claw back and up to the collection funnel
    servo.write(servoOpen); // Deposit the sample
    z_clockwise(); // Rotate to next sample
    servo.write(servoClosed); // Close the claw
  }
  else if(motor == 1){ // Horizontal Motor
    Serial.println(F("Horizontal Motor selected."));
    Serial.println(F("Choose Direction:\n1 - Forward\n2 - Backward\nChoice: "));
    direction = Serial.parseInt();
    if (direction == 1){
      Serial.println(F("Forward selected. Driving..."));
      x_forward();
    }
    else if (direction == 2){
      Serial.println(F("Backward selected. Driving..."));
      x_backward();
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
      y_forward();
    }
    else if (direction == 2){
      Serial.println(F("Upward selected. Driving..."));
      y_backward();
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
      z_clockwise();
    }
    else if (direction == 2){
      Serial.println(F("Counterclockwise selected. Driving..."));
      z_counterclockwise();
    }
    else{
      Serial.println(F("Incorrect option entered. Please try again.\n"));
    }
  }
}

void x_forward(){
  Serial.println(F("Running clockwise..."));
 	digitalWrite(dirXPin, HIGH); // Enables the motor to move in a particular direction
 	// Makes 200 pulses for making one full cycle rotation
 	for (int i = 0; i < x_steps; i++) {
 			digitalWrite(stepXPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepXPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}

void x_backward(){
  Serial.println(F("Running counter-clockwise..."));
 	digitalWrite(dirXPin, LOW); //Changes the rotations direction
 	// Makes 400 pulses for making two full cycle rotation
 	for (int i = 0; i < x_steps; i++) {
 			digitalWrite(stepXPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepXPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}

void y_forward(){
  Serial.println(F("Running clockwise..."));
 	digitalWrite(dirYPin, HIGH); // Enables the motor to move in a particular direction
 	// Makes 200 pulses for making one full cycle rotation
 	for (int i = 0; i < y_steps; i++) {
 			digitalWrite(stepYPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepYPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}

void y_backward(){
  Serial.println(F("Running counter-clockwise..."));
 	digitalWrite(dirYPin, LOW); //Changes the rotations direction
 	// Makes 400 pulses for making two full cycle rotation
 	for (int i = 0; i < y_steps; i++) {
 			digitalWrite(stepYPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepYPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}

void z_clockwise(){
  Serial.println(F("Running clockwise..."));
  digitalWrite(dirZPin, LOW); //Changes the rotations direction
 	// Makes 400 pulses for making two full cycle rotation
 	for (int i = 0; i < z_steps; i++) {
 			digitalWrite(stepZPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepZPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}

void z_counterclockwise(){
  Serial.println(F("Running counter-clockwise..."));
 	digitalWrite(dirZPin, HIGH); // Enables the motor to move in a particular direction
 	// Makes 200 pulses for making one full cycle rotation
 	for (int i = 0; i < z_steps; i++) {
 			digitalWrite(stepZPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepZPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}

void h_v_abduction(){
  Serial.println(F("Running horizontal and vertical motors in abduction..."));
  digitalWrite(dirXPin, HIGH);
  digitalWrite(dirYPin, HIGH);

  // Move the horizotal and vertical motors together to pick up a sample
 	for (int i = 0; i < y_steps; i++) {
      unsigned int proxValue = proximitySensor.getProximity(); // VCNL4040 Proximity Sensor
      if(proxValue > proxThresh) {
        break;
      }
      if(i < x_steps){
 			  digitalWrite(stepXPin, HIGH); // Horizontal motor steps will stop first
      }
      digitalWrite(stepYPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);

      if(i < x_steps){
 			  digitalWrite(stepXPin, LOW); // Horizontal motor steps will stop first
      }
      digitalWrite(stepYPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}

void h_v_adduction(){
  Serial.println(F("Running horizontal and vertical motors in adduction..."));
  digitalWrite(dirXPin, LOW);
  digitalWrite(dirYPin, LOW);

  // Move the horizotal and vertical motors together to retrieve a sample
 	for (int i = 0; i < y_steps; i++) {
      unsigned int proxValue = proximitySensor.getProximity(); // VCNL4040 Proximity Sensor
      if(proxValue > proxThresh) {
        break;
      }
      if(i < x_steps){
 			  digitalWrite(stepXPin, HIGH); // Horizontal motor steps will stop first
      }
      digitalWrite(stepYPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
      
      if(i < x_steps){
 			  digitalWrite(stepXPin, LOW); // Horizontal motor steps will stop first
      }
      digitalWrite(stepYPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
}