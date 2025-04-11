#include <Servo.h>  // Include the Servo library for controlling servo motors

Servo myservo;  // Create a Servo object to control the servo motor

// Define L298N motor driver pins
const int ENA = 5;  // ENA pin (PWM) for controlling motor speed
const int IN1 = 6;  // IN1 pin for controlling motor direction
const int IN2 = 7;  // IN2 pin for controlling motor direction

// State variables
bool flipped = false;   // Keeps track of servo swing direction (false: 0→180, true: 180→0)
bool started = false;   // Flag to indicate if the system is running (triggered via serial input)

void setup() {
  Serial.begin(9600);  // Initialize serial communication at 9600 baud

  // Initialize the servo on pin D9 and set its starting angle to 0 degrees
  myservo.attach(9);
  myservo.write(0);

  // Set motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Stop the motor initially
  analogWrite(ENA, 0);

  // Print user instructions to the serial monitor
  Serial.println("Type 'start' to run, 'stop' to halt");
}

void loop() {
  // Check for incoming serial input
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');  // Read the input until a newline
    command.trim();  // Remove leading/trailing whitespace or line endings

    // Start command received
    if (command == "start") {
      started = true;
      Serial.println("System started: Motor + Servo running...");
    }
    // Stop command received
    else if (command == "stop") {
      started = false;
      Serial.println("System stopped.");

      // Stop the motor immediately
      analogWrite(ENA, 0);
    }
  }

  // If system is running, perform motor and servo actions
  if (started) {
    // Set motor to rotate forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 255);  // Set motor speed to maximum

    // Servo swing logic
    if (flipped) {
      // Move servo from 180° to 0°
      for (int i = 180; i >= 0; i--) {
        if (!started) break;  // Interrupt swing if system is stopped mid-way
        myservo.write(i);     // Set servo to angle i
        delay(5);             // Delay for smooth movement
      }
    } else {
      // Move servo from 0° to 180°
      for (int i = 0; i <= 180; i++) {
        if (!started) break;
        myservo.write(i);
        delay(5);
      }
    }

    flipped = !flipped;  // Flip direction for next swing
    delay(300);          // Small pause between swings
  }
}

