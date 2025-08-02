# Servo-Controlled Walking Robot

This repository contains code for controlling a servo-driven bipedal robot using ESP32 and PCA9685 servo driver via Bluetooth commands. It allows the robot to walk forward, backward, and make left and right turns smoothly.

## Hardware Components

* **ESP32 microcontroller**
* **Adafruit PCA9685 PWM Servo Driver**
* **Servo Motors** (12 servos controlling hips, knees, ankles, feet, and rotations)
* **Bluetooth module (ESP32 built-in)**

## Libraries Required

* `Wire.h` (included in Arduino IDE)
* `Adafruit_PWMServoDriver.h` (install via Arduino IDE Library Manager)
* `BluetoothSerial.h` (ESP32 built-in)

## Servo Configuration

Servos are connected via PCA9685 using channels:

| Servo Name      | Channel |
| --------------- | ------- |
| LEFT\_ROTATION  | 0       |
| LEFT\_HIP       | 1       |
| LEFT\_THIGH     | 2       |
| LEFT\_KNEE      | 3       |
| LEFT\_ANKLE     | 4       |
| LEFT\_FOOT      | 5       |
| RIGHT\_ROTATION | 10      |
| RIGHT\_HIP      | 11      |
| RIGHT\_THIGH    | 12      |
| RIGHT\_KNEE     | 13      |
| RIGHT\_ANKLE    | 14      |
| RIGHT\_FOOT     | 15      |

## Features

* **Bluetooth Control:**

  * `F`: Walk forward
  * `B`: Walk backward
  * `L`: Turn left
  * `R`: Turn right
  * `S`: Stop and reset to initial position

* **Smooth Movements:**

  * Servos transition gradually between angles for fluid and natural movements.

## Functions

* **Initial Position:** `Initial_Pos()` sets the robot to its stable starting stance.
* **Movement Commands:**

  * `Walk_Forward()`
  * `Walk_Backward()`
  * `Turn_Left()`
  * `Turn_Right()`
* **Servo Movement:**

  * `moveServosGradually()` for smooth transitions
