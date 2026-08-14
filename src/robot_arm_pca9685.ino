/*
  Robot Arm PCA9685 Controller
  ---------------------------------
  Controls multiple servos using the PCA9685 16-channel PWM driver.
  Features a single push button to cycle through and select active servos.

  Hardware Requirements:
  - Arduino Uno / Nano
  - PCA9685 I2C Servo Driver (0x40)
  - Servos (e.g., SG90 / MG996R) connected to PWM channels
  - Push Button with 10k Ohm pull-down resistor on Pin 2
  - External 5V Power Supply connected to PCA9685 V+
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Initialize PCA9685 at default I2C address (0x40)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Servo Pulse Range Settings (Adjust according to your servo specs)
#define SERVOMIN  150 // Minimum pulse length count (out of 4096)
#define SERVOMAX  600 // Maximum pulse length count (out of 4096)
#define SERVO_FREQ 50 // Standard analog servo frequency (50Hz)

// Hardware Pins & Config
const int BUTTON_PIN = 2;       // Digital Input Pin for servo toggle button
const int TOTAL_SERVOS = 4;     // Number of servos connected (Channels 0 to 3)

// State Variables
int selectedServo = 0;
bool lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // 50ms debounce time

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);

  // Initialize PCA9685 Driver
  pwm.begin();
  pwm.setOscillatorFrequency(27000000); // 27 MHz internal oscillator
  pwm.setPWMFreq(SERVO_FREQ);

  delay(10);
  
  Serial.println("==========================================");
  Serial.println("   Robot Arm PCA9685 Controller Started   ");
  Serial.println("==========================================");
  Serial.print("Currently selected servo channel: ");
  Serial.println(selectedServo);
}

void loop() {
  // Read physical button state
  bool currentReading = digitalRead(BUTTON_PIN);

  // Debounce check
  if (currentReading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    static bool buttonState = LOW;
    if (currentReading != buttonState) {
      buttonState = currentReading;

      // Single-button press action (triggers on HIGH)
      if (buttonState == HIGH) {
        selectedServo = (selectedServo + 1) % TOTAL_SERVOS;
        
        Serial.print("[BTN] Active Servo Channel Switched -> ");
        Serial.println(selectedServo);
      }
    }
  }

  lastButtonState = currentReading;

  // Position control or potentiometer read logic for selectedServo goes here
}
