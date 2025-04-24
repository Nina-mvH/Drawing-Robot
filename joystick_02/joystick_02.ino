#include <Stepper.h>

//pin numbers for joystick
const int VRx = A0;  
const int VRy = A1; 
const int SW = 7; 

const int toolChangePower = 17; //pin number for tool changer DC motor
const int button = 31; // pin number for the button (change tool)

const int threshold = 100; //direction threshold

//establish stepper motors
const int stepsPerRevolution = 200;   
Stepper motorA(stepsPerRevolution, 2, 3, 4, 5);
Stepper motorB(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  Serial.begin(9600);
  pinMode(SW, INPUT_PULLUP);

  motorA.setSpeed(255); // RPM
  motorB.setSpeed(255);

  //initialize tool changer pins
  pinMode(toolChangePower, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  int xVal = analogRead(VRx); //read how far joystick is pushed along the x axis
  int yVal = analogRead(VRy); //read how far joystick is pushed along the y axis
  int switchState = digitalRead(SW); //read if the joystick has been pressed down

  String direction = "";
  bool moved = false;

  //check direction (l/r) for the x axis
  if (xVal < (512 - threshold)) {
    direction += "Left ";
    motorA.step(1);
    motorB.step(1);
    moved = true;
  } else if (xVal > (512 + threshold)) {
    direction += "Right ";
    motorA.step(-1);
    motorB.step(-1);
    moved = true;
  }

  //check direction (u/d) for the y axis
  if (yVal < (512 - threshold)) {
    direction += "Up ";
    motorA.step(1);
    motorB.step(-1);
    moved = true;
  } else if (yVal > (512 + threshold)) {
    direction += "Down ";
    motorA.step(-1);
    motorB.step(1);
    moved = true;
  }

  if (moved) {
    Serial.print("Direction: ");
    Serial.println(direction);
    delay(5);
  }

  //check if the joystick button was pushed
  if (switchState == LOW) {
    Serial.println("Joystick clicked!");
    //lift the servo if down, lower if up
    delay(200);
  }

  int buttonState = digitalRead(button);
  if (buttonState == HIGH) {
    changeTool();
  }

  // delay(50);
}

void changeTool() {
  Serial.println("Button Pressed!");
  digitalWrite(toolChangePower, HIGH);
  delay(400);
  digitalWrite(toolChangePower, LOW);
}