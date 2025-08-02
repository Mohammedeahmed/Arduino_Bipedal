#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT; // Create a BluetoothSerial object

// Create the PCA9685 object using the default I2C address (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo parameters
#define SERVOMIN 150 // Minimum pulse length count (for 0 degrees)
#define SERVOMAX 600 // Maximum pulse length count (for 270 degrees)

// Define servo names and their corresponding channels
#define RIGHT_ROTATION 10
#define RIGHT_HIP 11
#define RIGHT_THIGH 12
#define RIGHT_KNEE 13
#define RIGHT_ANKLE 14
#define RIGHT_FOOT 15

#define LEFT_ROTATION 0
#define LEFT_HIP 1
#define LEFT_THIGH 2
#define LEFT_KNEE 3
#define LEFT_ANKLE 4
#define LEFT_FOOT 5

// Function to map angle (0-270) to pulse length
int angleToPulse(int angle) {
  return map(angle, 0, 270, SERVOMIN, SERVOMAX);
}

void Initial_Pos(){
  pwm.setPWM(RIGHT_ROTATION, 0, angleToPulse(90)); //90
  pwm.setPWM(RIGHT_HIP, 0, angleToPulse(87));      //87 
  pwm.setPWM(RIGHT_THIGH, 0, angleToPulse(107));   //109  
  pwm.setPWM(RIGHT_KNEE, 0, angleToPulse(114));    //114   
  pwm.setPWM(RIGHT_ANKLE, 0, angleToPulse(95));    //95   
  pwm.setPWM(RIGHT_FOOT, 0, angleToPulse(85));     //85

  pwm.setPWM(LEFT_ROTATION, 0, angleToPulse(76)); //76
  pwm.setPWM(LEFT_HIP, 0, angleToPulse(82));      //83 
  pwm.setPWM(LEFT_THIGH, 0, angleToPulse(72));    //70 
  pwm.setPWM(LEFT_KNEE, 0, angleToPulse(65));     //65 
  pwm.setPWM(LEFT_ANKLE, 0, angleToPulse(75));    //75  
  pwm.setPWM(LEFT_FOOT, 0, angleToPulse(89));     //89 

}

// Smooth servo movement from initial to target angles
void moveServosGradually(int channels[], int initialAngles[], int targetAngles[], int numServos, int duration) {
  int steps = 50; // Number of intermediate steps
  int stepDelay = duration / steps; // Delay per step (ms)

  for (int step = 0; step <= steps; step++) {
    for (int i = 0; i < numServos; i++) {
      int currentAngle = initialAngles[i] + ((targetAngles[i] - initialAngles[i]) * step) / steps;
      pwm.setPWM(channels[i], 0, angleToPulse(currentAngle));
    }
    delay(stepDelay);
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Robot"); // Bluetooth name
  Serial.println("Bluetooth is ready to pair.");
  Serial.println("Initializing PCA9685 and Servo...");
  pwm.begin();
  pwm.setPWMFreq(50); // Set PWM frequency to 50 Hz
  Initial_Pos();
  Serial.println("Initialization complete.");
}

void loop() {
  if (SerialBT.available()) {
    char command = SerialBT.read(); // Read the command from Bluetooth
    Serial.println(command);        // Debug: Print the command received

    switch (command) {
      case 'F': // Forward
        Walk_Forward();
        break;
      case 'B': // Forward
        Walk_Backward();
        break;
      case 'L': // Forward
        Turn_Left();
        break;
      case 'R': // Forward
        Turn_Right();
        break;
      case 'S':
        Initial_Pos();
        break;
    }
  }
}


//_____________________________________________________WALKING FORWARD___________________________________________________________________

int Walking_Speed = 250;

void Walk_Forward() {
  while (true) { // Infinite loop for continuous walking
    if (SerialBT.available()) {
      char command = SerialBT.read(); // Check for new commands
      if (command == 'S') { // Stop walking
        Initial_Pos();
        return; // Exit the function
      }
    }
    // Perform one cycle of walking
    Right_Tilt();
    Left_Leg_Up__L_forward__R_forward_return();
    Right_Tilt_Return__Left_Leg_Down();
    Left_Tilt();
    Right_Leg_Up__R_forward__L_forward_return();
    Left_Tilt_Return__Right_Leg_Down();
  }
}


// Robot motions
void Right_Tilt() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT};
  int initialAngles[] = {86, 82, 85, 89};
  int targetAngles[] = {78, 74, 73, 77};
  moveServosGradually(channels, initialAngles, targetAngles, 4, Walking_Speed);
}

void Right_Tilt_Return__Left_Leg_Down() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT, LEFT_THIGH, LEFT_KNEE};
  int initialAngles[] = {78, 74, 73, 77, 35, 30};
  int targetAngles[] = {86, 82, 85, 89, 70, 65};
  moveServosGradually(channels, initialAngles, targetAngles, 6, Walking_Speed);
}

void Left_Tilt() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT};
  int initialAngles[] = {86, 82, 85, 89};
  int targetAngles[] = {94, 90, 101, 105};
  moveServosGradually(channels, initialAngles, targetAngles, 4, Walking_Speed);
}

void Left_Tilt_Return__Right_Leg_Down() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT, RIGHT_THIGH, RIGHT_KNEE};
  int initialAngles[] = {94, 90, 101, 105, 144, 149};
  int targetAngles[] = {86, 82, 85, 89, 109, 114};
  moveServosGradually(channels, initialAngles, targetAngles, 6, Walking_Speed);
}

void Left_Leg_Up__L_forward__R_forward_return(){
  int channels[] = {LEFT_THIGH, LEFT_KNEE, RIGHT_THIGH, RIGHT_ANKLE, LEFT_ANKLE};
  int initialAngles[] = {70, 65, 109, 95, 65};
  int targetAngles[] = {35, 30, 99, 105, 75};
  moveServosGradually(channels, initialAngles, targetAngles, 5, Walking_Speed);
  
}

void Right_Leg_Up__R_forward__L_forward_return(){
  int channels[] = {RIGHT_THIGH, RIGHT_KNEE, LEFT_THIGH, LEFT_ANKLE, RIGHT_ANKLE};
  int initialAngles[] = {109, 114, 70, 75, 105};
  int targetAngles[] = {144, 149, 80, 65, 95};
  moveServosGradually(channels, initialAngles, targetAngles, 5, Walking_Speed);
  
}
//________________________________________________________________________________________________________________________________________________________


//_____________________________________________________LEFT TURN___________________________________________________________________

int Turn_Speed = 250;

void Turn_Left() {
  while (true) { // Infinite loop for continuous walking
    if (SerialBT.available()) {
      char command = SerialBT.read(); // Check for new commands
      if (command == 'S') { // Stop walking
        Initial_Pos();
        return; // Exit the function
      }
    }
    // Perform one cycle of Turning Left
    Left_turn_1();
    Left_turn_2();
    Left_turn_3();
    Left_turn_4();
    Left_turn_5();
    Left_turn_6();
  }
}


//Tilt the robot to the right.
void Left_turn_1() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT};
  int initialAngles[] = {87, 83, 85, 89};
  int targetAngles[] = {77, 73, 69, 73};
  moveServosGradually(channels, initialAngles, targetAngles, 4, Turn_Speed);
}

//Lift the left leg up and turns the body to the left.
void Left_turn_2(){
  int channels[] = {LEFT_THIGH, LEFT_KNEE, RIGHT_ROTATION, LEFT_ROTATION};
  int initialAngles[] = {70, 65, 90, 74};
  int targetAngles[] = {35, 30, 110, 54};
  moveServosGradually(channels, initialAngles, targetAngles, 4, Turn_Speed);
  
}

//From right tilt returns to Initial Position and Puts the Left Leg down.
void Left_turn_3() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT, LEFT_THIGH, LEFT_KNEE};
  int initialAngles[] = {77, 73, 69, 73, 35, 30};
  int targetAngles[] = {87, 83, 85, 89, 70, 65};
  moveServosGradually(channels, initialAngles, targetAngles, 6, Turn_Speed);
}

//Tilt the robot to the left.
void Left_turn_4() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT};
  int initialAngles[] = {87, 83, 85, 89};
  int targetAngles[] = {97, 93, 101, 105};
  moveServosGradually(channels, initialAngles, targetAngles, 4, Turn_Speed);
}

////Lift the right leg up and return the body to the initial position.
void Left_turn_5(){
  int channels[] = {RIGHT_THIGH, RIGHT_KNEE, RIGHT_ROTATION, LEFT_ROTATION};
  int initialAngles[] = {109, 114, 110, 54};
  int targetAngles[] = {144, 149, 90, 74};
  moveServosGradually(channels, initialAngles, targetAngles, 4, Turn_Speed);
  
}

//From left tilt returns to Initial Position and Puts the right leg down.
void Left_turn_6() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT, RIGHT_THIGH, RIGHT_KNEE};
  int initialAngles[] = {97, 93, 101, 105, 144, 149};
  int targetAngles[] = {87, 83, 85, 89, 109, 114};
  moveServosGradually(channels, initialAngles, targetAngles, 6, Turn_Speed);
}

//________________________________________________________________________________________________________________________________________________________

//_____________________________________________________RIGHT TURN___________________________________________________________________

void Turn_Right() {
  while (true) { // Infinite loop for continuous walking
    if (SerialBT.available()) {
      char command = SerialBT.read(); // Check for new commands
      if (command == 'S') { // Stop walking
        Initial_Pos();
        return; // Exit the function
      }
    }
    // Perform one cycle of Turning Left
    Right_turn_1();
    Right_turn_2();
    Right_turn_3();
    Right_turn_4();
    Right_turn_5();
    Right_turn_6();
  }
}


//Tilt the robot to the left.
void Right_turn_1() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT};
  int initialAngles[] = {87, 83, 85, 89};
  int targetAngles[] = {97, 93, 101, 105};
  moveServosGradually(channels, initialAngles, targetAngles, 4, Turn_Speed);
}

////Lift the right leg up and turn the body to right.
void Right_turn_2(){
  int channels[] = {RIGHT_THIGH, RIGHT_KNEE, RIGHT_ROTATION, LEFT_ROTATION};
  int initialAngles[] = {109, 114, 90, 74};
  int targetAngles[] = {144, 149, 110, 54};
  moveServosGradually(channels, initialAngles, targetAngles, 4, Turn_Speed);
  
}

//From left tilt returns to Initial Position and Puts the right leg down.
void Right_turn_3() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT, RIGHT_THIGH, RIGHT_KNEE};
  int initialAngles[] = {97, 93, 101, 105, 144, 149};
  int targetAngles[] = {87, 83, 85, 89, 109, 114};
  moveServosGradually(channels, initialAngles, targetAngles, 6, Turn_Speed);
}


//Tilt the robot to the right.
void Right_turn_4() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT};
  int initialAngles[] = {87, 83, 85, 89};
  int targetAngles[] = {77, 73, 69, 73};
  moveServosGradually(channels, initialAngles, targetAngles, 4, Turn_Speed);
}

//Lift the left leg up and return the boby to the inintial position.
void Right_turn_5(){
  int channels[] = {LEFT_THIGH, LEFT_KNEE, RIGHT_ROTATION, LEFT_ROTATION};
  int initialAngles[] = {70, 65, 110, 54};
  int targetAngles[] = {35, 30, 90, 74};
  moveServosGradually(channels, initialAngles, targetAngles, 4, Turn_Speed);
  
}

//From right tilt returns to Initial Position and Puts the Left Leg down.
void Right_turn_6() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT, LEFT_THIGH, LEFT_KNEE};
  int initialAngles[] = {77, 73, 69, 73, 35, 30};
  int targetAngles[] = {87, 83, 85, 89, 70, 65};
  moveServosGradually(channels, initialAngles, targetAngles, 6, Turn_Speed);
}

//________________________________________________________________________________________________________________________________________________________

//_____________________________________________________WALKING BACKWARD___________________________________________________________________


void Walk_Backward() {
  while (true) { // Infinite loop for continuous walking
    if (SerialBT.available()) {
      char command = SerialBT.read(); // Check for new commands
      if (command == 'S') { // Stop walking
        Initial_Pos();
        return; // Exit the function
      }
    }
    // Perform one cycle of walking
    Backward_1();
    Backward_2();
    Backward_3();
    Backward_4();
    Backward_5();
    Backward_6();
    
  }
}


// Tilt the robot to the right
void Backward_1() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT};
  int initialAngles[] = {87, 83, 85, 89};
  int targetAngles[] = {77, 73, 69, 73};
  moveServosGradually(channels, initialAngles, targetAngles, 4, Walking_Speed);
}

//lift the left leg and push the left leg back, also return the right forward to its initial position
void Backward_2(){
  int channels[] = {LEFT_THIGH, LEFT_KNEE, RIGHT_THIGH, RIGHT_ANKLE, LEFT_ANKLE};
  int initialAngles[] = {70, 65, 109, 95, 85};
  int targetAngles[] = {35, 30, 119, 85, 75};
  moveServosGradually(channels, initialAngles, targetAngles, 5, Walking_Speed);
  
}

// return the right tilt to its iniitial position and put the left leg down
void Backward_3() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT, LEFT_THIGH, LEFT_KNEE};
  int initialAngles[] = {77, 73, 69, 73, 35, 30};
  int targetAngles[] = {87, 83, 85, 89, 70, 65};
  moveServosGradually(channels, initialAngles, targetAngles, 6, Walking_Speed);
}

//Tilt the robot to the left
void Backward_4() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT};
  int initialAngles[] = {87, 83, 85, 89};
  int targetAngles[] = {97, 93, 101, 105};
  moveServosGradually(channels, initialAngles, targetAngles, 4, Walking_Speed);
}

//Lift the right leg up and push the right leg back, also return the left forward to its initial position
void Backward_5(){
  int channels[] = {RIGHT_THIGH, RIGHT_KNEE, LEFT_THIGH, LEFT_ANKLE, RIGHT_ANKLE};
  int initialAngles[] = {109, 114, 70, 75, 85};
  int targetAngles[] = {144, 149, 60, 85, 95};
  moveServosGradually(channels, initialAngles, targetAngles, 5, Walking_Speed);
  
}

//Return the left tilt to its initial position and put the right leg down.
void Backward_6() {
  int channels[] = {RIGHT_HIP, LEFT_HIP, RIGHT_FOOT, LEFT_FOOT, RIGHT_THIGH, RIGHT_KNEE};
  int initialAngles[] = {97, 93, 101, 105, 144, 149};
  int targetAngles[] = {87, 83, 85, 89, 109, 114};
  moveServosGradually(channels, initialAngles, targetAngles, 6, Walking_Speed);
}




//________________________________________________________________________________________________________________________________________________________
