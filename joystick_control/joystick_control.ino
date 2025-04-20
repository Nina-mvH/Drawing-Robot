// joystick pins
const int VRx = A3;
const int VRy = A4;
const int SW = 16;

const int threshold = 100;

// stepper pins
#define LEFT_STEP_PIN 60
#define LEFT_DIR_PIN 61
#define LEFT_ENABLE_PIN 56

#define RIGHT_STEP_PIN 46
#define RIGHT_DIR_PIN 48
#define RIGHT_ENABLE_PIN 62

#define TOOL_CHANGE_POWER 17

// #define BUTTON_PIN 23
// int buttonState = 0;


void setup() {
  Serial.begin(9600);
  // initialize stepper motors
  pinMode(LEFT_STEP_PIN, OUTPUT);
  pinMode(LEFT_DIR_PIN, OUTPUT);
  pinMode(LEFT_ENABLE_PIN, OUTPUT);

  pinMode(RIGHT_STEP_PIN, OUTPUT);
  pinMode(RIGHT_DIR_PIN, OUTPUT);
  pinMode(RIGHT_ENABLE_PIN, OUTPUT);

  digitalWrite(LEFT_ENABLE_PIN, LOW);
  digitalWrite(RIGHT_ENABLE_PIN, LOW);

  // initialize joystick button
  pinMode(SW, INPUT_PULLUP);

  //initialize tool change motor
  pinMode(TOOL_CHANGE_POWER, OUTPUT);

  //button
  // pinMode(BUTTON_PIN, INPUT);
  

}

void loop() {
  int xVal = analogRead(VRx);
  int yVal = analogRead(VRy);
  int switchState = digitalRead(SW);

  bool moved = false;

  // move up
  if (yVal < (512 - threshold)) {
    moveBoth(true, true);  // both shorten
    moved = true;
  }

  // move down
  else if (yVal > (512 + threshold)) {
    moveBoth(false, false);  // both lengthen
    moved = true;
  }

  // move left
  if (xVal < (512 - threshold)) {
    moveBoth(false, true);  // left lengthens, right shortens
    moved = true;
  }

  // move right
  else if (xVal > (512 + threshold)) {
    moveBoth(true, false);  // left shortens, right lengthens
    moved = true;
  }

  if (switchState == LOW) {
    Serial.println("Switch clicked!");
    changeTool();
    delay(200);
  }

  // // read the state of the pushbutton value:
  // buttonState = digitalRead(BUTTON_PIN);

  // // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  // if (buttonState == HIGH) {
  //   Serial.println("Button Pressed!");
  // }
  // } else {
  //   Serial.println("No Press!");
  // }

  // if (!moved) {
  //   delay(10);  // pause when idle
  // }
}

void moveBoth(bool leftDir, bool rightDir) {
  digitalWrite(LEFT_DIR_PIN, leftDir);
  digitalWrite(RIGHT_DIR_PIN, rightDir);

  digitalWrite(LEFT_STEP_PIN, HIGH);
  digitalWrite(RIGHT_STEP_PIN, HIGH);

  digitalWrite(LEFT_STEP_PIN, LOW);
  digitalWrite(RIGHT_STEP_PIN, LOW);
}

void changeTool() {
  Serial.println("Joystick clicked!");
  digitalWrite(TOOL_CHANGE_POWER, HIGH);
  delay(400);
  digitalWrite(TOOL_CHANGE_POWER, LOW);
}


