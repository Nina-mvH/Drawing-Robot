#include <Stepper.h>

const int VRx = A0;
const int VRy = A1;
const int SW = 7;

const int threshold = 100;
const int stepsPerRevolution = 200;  

Stepper motorA(stepsPerRevolution, 2, 3, 4, 5);
Stepper motorB(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  Serial.begin(9600);
  pinMode(SW, INPUT_PULLUP);

  motorA.setSpeed(255); // RPM
  motorB.setSpeed(255);
}

void loop() {
  int xVal = analogRead(VRx);
  int yVal = analogRead(VRy);
  int switchState = digitalRead(SW);

  String direction = "";
  bool moved = false;

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

  if (switchState == LOW) {
    Serial.println("Switch clicked!");
    delay(200);
  }

  // delay(50);
}
