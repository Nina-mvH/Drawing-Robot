#include <Stepper.h>
#include <Servo.h>

//JOYSTICK FOR CONTROLLING THE PLOTTER
const int VRx = A0;  
const int VRy = A1; 
const int SW = 7; 
const int threshold = 100; //direction threshold
int xVal; //read how far joystick is pushed along the x axis
int yVal; //read how far joystick is pushed along the y axis
int switchState; //read if the joystick has been pressed down

//STEPPER MOTORS FOR MOVING THE PLOTTER
const int stepsPerRevolution = 200;   
Stepper motorA(stepsPerRevolution, 2, 3, 4, 5);
Stepper motorB(stepsPerRevolution, 8, 9, 10, 11);
bool moved; //keep track if plotter moved
int stepA; //step count to be sent to motorA
int stepB; //step count to be sent to motorB

//DC MOTOR FOR CHANGING TOOLS
const int toolChangeF = 30; 
const int toolChangeB = 32; 

//SERVO FOR LIFTING THE PEN
Servo myservo; //initialize myservo as a Servo object
const int servoSignal = 33; //the pin that the servo motor is attached to 
bool up; //a boolean to keep track of the servo motor's arm position

//BUTTON FOR ACTIVATING THE SERVO
const int buttonPin = 31;  // the number of the button pin
int buttonState = 0;  // variable for reading the button status

void setup() {
  Serial.begin(9600);
  pinMode(SW, INPUT_PULLUP); //joystick

  //STEPPER MOTORS
  motorA.setSpeed(40); // RPM
  motorB.setSpeed(40); // RPM

  //TOOL CHANGER DC
  pinMode(toolChangeF, OUTPUT); //establishes the DC motor forward direction pin as an output
  pinMode(toolChangeB, OUTPUT); //establishes the DC motor backward direction pin as an output

  //SERVO AND BUTTON
  pinMode(servoSignal, OUTPUT); //initalize servo
  myservo.attach(servoSignal); //initalize servo
  myservo.write(180); // set servo posiiton to up
  up = true; // record servo position as up
  pinMode(buttonPin, INPUT); //initialize button
}



void loop() {
  xVal = analogRead(VRx); //read how far joystick is pushed along the x axis
  yVal = analogRead(VRy); //read how far joystick is pushed along the y axis
  switchState = digitalRead(SW); //read if the joystick has been pressed down

  String direction = ""; //for printing/debugging
  moved = false;

  stepA = 0;
  stepB = 0;


  //check direction (l/r) for the x axis and rotate stepper motors
  if (yVal > (512 + threshold)) {
    direction += "Left ";
    stepA +=2;
    stepB +=2;
  } else if (yVal < (512 - threshold)) {
    direction += "Right ";
    stepA -=2;
    stepB -=2;
  }
  //check direction (u/d) for the y axis and rotate stepper motors
  if (xVal < (512 - threshold)) {
    direction += "Up ";
    stepA +=1;
    stepB -=1;
  } else if (xVal > (512 + threshold)) {
    direction += "Down ";
    stepA -=1;
    stepB +=1;
  }


  if ((stepA != 0) || (stepB != 0)) {
    motorA.step(stepA);
    motorB.step(stepB);
    moved = true;
  }
  if (moved) {
    Serial.print("Direction: ");
    Serial.println(direction);
    // delay(3);
  }

  //check if the joystick button was pushed
  if (switchState == LOW) {
    Serial.println("Joystick clicked!");
    changeTool();
    delay(200);
  }

  
  //https://docs.arduino.cc/built-in-examples/digital/Button/
  buttonState = digitalRead(buttonPin);// read the state of the pushbutton value
  if (buttonState == LOW) {
    Serial.println("Button clicked!");
    changeLines();
    delay(380);
  }

  // delay(50);
}

//method to change the tool using a timed DC motor
void changeTool() {
  Serial.println("Change tool");
  digitalWrite(toolChangeF, HIGH); //send HIGH to the pin associated with rotating the DC motor forwards
  delay(1000); //pause to allow for the tool to change
  digitalWrite(toolChangeF, LOW); //send LOW to the pin associated with rotating the DC motor forwards
}


//method to either lift/lower the plotter to draw seperate lines
void changeLines() {
  if (up) { //if the servo arm is up
    myservo.write(90); //move the servo arm down
    up = false; //keep track of servo arm position
  }
  else { //if the servo arm is down
    myservo.write(180); //move the servo arm up
    up = true; //keep track of servo arm position
  }
}