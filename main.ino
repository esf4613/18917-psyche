#include <Servo.h>
#include <Adafruit_VCNL4040.h>

// Servo variables
Servo servo;
const byte servoPin = A3; // coolant enable
byte pos = 0;
const int servoClosed = 0; // Servo angle closed
const int servoOpenCollection = 90; // Servo angle open at the collection site (larger for more sample)
const int servoOpenFunnel = 60; // Servo angle open at the funnel (smaller for less spillage)

// Stepper variables
const int enPin=8;
const int stepXPin = 2; //X.STEP
const int dirXPin = 5; // X.DIR
const int stepYPin = 3; //Y.STEP
const int dirYPin = 6; // Y.DIR
const int stepZPin = 4; //Z.STEP
const int dirZPin = 7; // Z.DIR

// SparkFun VCNL4040 Proximity Sensors
Adafruit_VCNL4040 vcnl4040 = Adafruit_VCNL4040(); // Sensor object
const int proxMin = 0; // Nothing is detected
const int proxMax = 65535; // Max value for detecting an object
const int proxThresh = 2; // Value for sensor proximity

const int x_steps=7300; // Horizontal stepper steps (200 per revolution)
const int x_steps_to_funnel=1300; // The number of steps from the base of the horiztonal motor to funnel
const int y_steps=9700; // Horizontal stepper steps (200 per revolution) [10,500 is highest value (max depth)] (use 9700 for box depth)
const int y_steps_to_funnel=2300; // The number of steps from the base of the vertical motor to funnel
const int z_steps=108; // Horizontal stepper steps (200 per revolution)

// Milliseconds between each step, higher value means a stronger drive, but slower
int x_microsBtwnSteps = 1500;
int y_microsBtwnSteps = 1000;
int z_millisBtwnSteps = 3000;
int pulseWidthMicros = 100; 	// microseconds

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

  // Proximity Sensor setup
  vcnl4040.begin();
}

void loop() {
  Serial.println(F("Motor Control Menu:\n1 - Horizontal\n2 - Vertical\n3 - Claw\n4 - Collection Cylinder\n5 - Demo Program\nChoice: "));
  char motor = Serial.parseInt();
  //char motor = 5;
  int direction;
  
  if(motor == 1){ // Horizontal Motor
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
      servo.write(servoOpenCollection);
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
  else if(motor == 5){ // Demo program
    Serial.println(F("Demo Program selected."));
    servo.write(servoOpenCollection); // Open the claw
    h_v_abduction(); // Move claw over and down to the soil
    servo.write(servoClosed); // Close the claw
    delay(1000); // Wait 1 second while collecting sample
    h_v_adduction(); // Move claw back and up to the collection funnel
    servo.write(servoOpenFunnel); // Deposit the sample
    delay(1000); // Wait 1 second while depositing sample
    xy_zero_out(); // Return x and y steppers to base (zeroed-out)
    z_clockwise(); // Rotate to next sample
    servo.write(servoClosed); // Close the claw
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
 			delayMicroseconds(x_microsBtwnSteps);
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
 			delayMicroseconds(x_microsBtwnSteps);
 	}
}

void x_backward_steps(int steps){
  Serial.println(F("Running counter-clockwise..."));
 	digitalWrite(dirXPin, LOW); //Changes the rotations direction
 	// Makes 400 pulses for making two full cycle rotation
 	for (int i = 0; i < steps; i++) {
 			digitalWrite(stepXPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepXPin, LOW);
 			delayMicroseconds(x_microsBtwnSteps);
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
 			delayMicroseconds(y_microsBtwnSteps);
 	}
}

void y_backward(){
 	digitalWrite(dirYPin, LOW); //Changes the rotations direction
 	// Makes 400 pulses for making two full cycle rotation
 	for (int i = 0; i < y_steps; i++) {
 			digitalWrite(stepYPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepYPin, LOW);
 			delayMicroseconds(y_microsBtwnSteps);
 	}
}

void y_backward_steps(int steps){
 	digitalWrite(dirYPin, LOW); //Changes the rotations direction
 	// Makes 400 pulses for making two full cycle rotation
 	for (int i = 0; i < steps; i++) {
 			digitalWrite(stepYPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepYPin, LOW);
 			delayMicroseconds(y_microsBtwnSteps);
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
 			delayMicroseconds(z_millisBtwnSteps);
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
 			delayMicroseconds(z_millisBtwnSteps);
 	}
}

// Moving downward
void h_v_abduction(){
  Serial.println(F("Running horizontal and vertical motors in abduction..."));
  digitalWrite(dirXPin, HIGH);
  digitalWrite(dirYPin, HIGH);

  // Move the horizotal and vertical motors together to pick up a sample
 	for (int i = 0; i < y_steps; i++) {
    if(i < x_steps){
      digitalWrite(stepXPin, HIGH); // Horizontal motor steps will stop first
    }
    digitalWrite(stepYPin, HIGH);
    delayMicroseconds(pulseWidthMicros);

    if(i < x_steps){
      digitalWrite(stepXPin, LOW); // Horizontal motor steps will stop first
    }
    digitalWrite(stepYPin, LOW);
    delayMicroseconds(x_microsBtwnSteps);
 	}
}

// Moving upward
void h_v_adduction(){
  Serial.println(F("Running horizontal and vertical motors in adduction..."));
  digitalWrite(dirXPin, LOW);
  digitalWrite(dirYPin, LOW);

  // Move the horizotal and vertical motors together to retrieve a sample
 	for (int i = 0; i < y_steps; i++) {
      if(i < ((x_steps+3200)-x_steps_to_funnel) && i >= 3200){ // Need vertical to go up first to avoid hitting claw on protrusion
 			  digitalWrite(stepXPin, HIGH); // Horizontal motor steps will stop first
      }
      if(i < (y_steps-y_steps_to_funnel)){ // Need vertical to stop so sample can be deposited close to funnel
        digitalWrite(stepYPin, HIGH);
      }
 			delayMicroseconds(pulseWidthMicros);
      
      if(i < ((x_steps+3200)-x_steps_to_funnel) && i >= 3200){
 			  digitalWrite(stepXPin, LOW); // Horizontal motor steps will stop first
      }
      if(i < (y_steps-y_steps_to_funnel)){
        digitalWrite(stepYPin, LOW);
      }
 			delayMicroseconds(x_microsBtwnSteps);
 	}
}

/* Used only in the demo program, assumes that x and y steppers are positioned over funnel */
void xy_zero_out(){
  // Move the horizotal and vertical motors back to zeroed position
 	for (int i = 0; i < y_steps_to_funnel; i++) {
      if(i < x_steps_to_funnel){
 			  digitalWrite(stepXPin, HIGH); // Horizontal motor steps will stop first
      }
      digitalWrite(stepYPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
      
      if(i < x_steps_to_funnel){
 			  digitalWrite(stepXPin, LOW); // Horizontal motor steps will stop first
      }
      digitalWrite(stepYPin, LOW);
 			delayMicroseconds(x_microsBtwnSteps);
 	}
}