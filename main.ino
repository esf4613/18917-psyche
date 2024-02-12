const int enPin=8;
const int stepXPin = 2; //X.STEP
const int dirXPin = 5; // X.DIR
const int stepYPin = 3; //Y.STEP
const int dirYPin = 6; // Y.DIR
const int stepZPin = 4; //Z.STEP
const int dirZPin = 7; // Z.DIR

int stepPin=stepYPin;
int dirPin=dirYPin;

const int stepsPerRev=200;
int pulseWidthMicros = 100; 	// microseconds
int millisBtwnSteps = 1000;

int motorMoveTime = 10000; // 10 Seconds

void setup() {
 	Serial.begin(9600);
 	pinMode(enPin, OUTPUT);
 	digitalWrite(enPin, LOW);
 	pinMode(stepPin, OUTPUT);
 	pinMode(dirPin, OUTPUT);
 
 	Serial.println(F("CNC Shield Initialized"));
}

void loop() {
  Serial.println(F("Motor Control Menu:\n1 - Horizontal\n2 - Vertical\n3 - Claw\n4 - Collection Cylinder\nChoice: "));
  motor = Serial.parseInt();

  if(motor == 1){ // Horizontal Motor
      Serial.println(F("Horizontal Motor selected."))
      Serial.println(F("Choose Direction:\n1 - Forward\n2 - Backward\nChoice: "));
      direction = Serial.parseInt();
      if (direction == 1){
        Serial.println(F("Forward selected. Driving..."));
        h_clockwise();
      }
      else if (Direction == 2){
        Serial.println(F("Backward selected. Driving..."));
        h_counterclockwise();
      }
      else{
        Serial.println(F("Incorrect option entered. Please try again.\n"));
      }

  } else if(motor == 2){ // Vertical Motor


  } else if(motor == 3){ // Claw
  
  
  } else if(motor == 4){ // Collection Cylinder


  } else{
    Serial.println(F("Incorrect option entered. Please try again.\n"));
    break;
  }




  h_direction = Serial.read();
}

void h_clockwise(){
  Serial.println(F("Running clockwise"));
 	digitalWrite(dirPin, HIGH); // Enables the motor to move in a particular direction
 	// Makes 200 pulses for making one full cycle rotation
 	for (int i = 0; i < stepsPerRev*5; i++) {
 			digitalWrite(stepPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
 	delay(motorMoveTime);
}

void h_counterclockwise(){
  Serial.println(F("Running counter-clockwise"));
 	digitalWrite(dirPin, LOW); //Changes the rotations direction
 	// Makes 400 pulses for making two full cycle rotation
 	for (int i = 0; i < 2*stepsPerRev*5; i++) {
 			digitalWrite(stepPin, HIGH);
 			delayMicroseconds(pulseWidthMicros);
 			digitalWrite(stepPin, LOW);
 			delayMicroseconds(millisBtwnSteps);
 	}
 	delay(motorMoveTime);
}