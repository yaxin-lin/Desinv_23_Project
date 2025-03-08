/*
Author: Yaxin Lin
Date: 03/07/2025

Description: 
This program controls an 8x8 WS2812B LED matrix and a dual-motor system using an L298N motor driver. 
The motors move in a circular motion while the LED matrix displays a red circle. The robot will rotate 
in a circular path while simultaneously lighting up an 8x8 matrix to visually represent the circle. 
After completing one full revolution, the motors stop, but the LED matrix retains the circle display.

Hardware Used:
- Arduino Uno
- WS2812B 8x8 LED Matrix
- L298N Motor Driver
- Two DC Motors

Functionality:
- Controls two DC motors to rotate in a circular pattern
- Displays a red circle on the LED matrix while moving
- Stops the motors automatically after completing a full rotation
- Maintains the LED display even after stopping the motors

Future Improvements:
- add more pattern
- Create animations on the LED matrix while moving
*/


#include <FastLED.h>

// MOTOR1 PINS
int ena = 5;
int in1 = 6;
int in2 = 7;
int in3 = 8;
int in4 = 9;
int enb = 10;

// WS2812B LED Matrix
#define LED_PIN     12   // WS2812B data pin
#define MATRIX_WIDTH  8   // width of matrix
#define MATRIX_HEIGHT 8   // height of matrix
#define NUM_LEDS    (MATRIX_WIDTH * MATRIX_HEIGHT)  // total LED
#define BRIGHTNESS  100  // LED light

CRGB leds[NUM_LEDS];

// 8x8 circle pattern (1=light，0=close)
byte circlePattern[8] = {
  0b00011000,  //    ██    
  0b01111110,  //   ████   
  0b01111110,  //  ██████  
  0b11111111,  // ████████ 
  0b11111111,  // ████████ 
  0b01111110,  //  ██████  
  0b01111110,  //   ████   
  0b00011000   //    ██    
};

// Calculate pixel index (convert 8x8 matrix to WS2812B linear index)
int XY(int x, int y) {
  return (y % 2 == 0) ? (y * MATRIX_WIDTH + x) : (y * MATRIX_WIDTH + (MATRIX_WIDTH - 1 - x));
}

void setup() {
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  
  drawCircle();
}

void loop() {
  // Execution completes after one full circle; no continuous looping.
}

void turnRight(int speedL, int speedR) {
  analogWrite(ena, speedL);
  analogWrite(enb, speedR);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ena, 0);
  analogWrite(enb, 0);
}

void drawCircle() {
  for (int i = 0; i < 360; i++) {
    turnRight(255, 200);  // Increase the speed of the left and right motors to increase power
    displayCircleOnLED(); // show LED circle
    delay(50);
  }
  stopMotors();
}

// Draw a red circle on the 8x8 WS2812B LED matrix
void displayCircleOnLED() {
  for (int y = 0; y < MATRIX_HEIGHT; y++) {
    for (int x = 0; x < MATRIX_WIDTH; x++) {
      if (circlePattern[y] & (1 << (7 - x))) {
        leds[XY(x, y)] = CRGB::Red;  // Set to red
      } else {
        leds[XY(x, y)] = CRGB::Black; // close LED
      }
    }
  }
  FastLED.show(); // Reset LED matrix
}
