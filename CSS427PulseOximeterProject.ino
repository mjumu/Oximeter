#include <Arduino.h>

// Assuming the necessary libraries for LCD and sensor handling are included
// #include <LiquidCrystal_I2C.h>

// Constants for pin assignments and thresholds
const int sensorPin = A0;
const int RED_LED_PIN = 10;
const int IR_LED_PIN = 11;
const float SENSOR_THRESHOLD = 200.0;

// Class Definitions
class DSP {
public:
    float readLight(int pin) {
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(IR_LED_PIN, HIGH);
        delay(5); // Stabilization delay
        float reading = analogRead(pin);
        digitalWrite(IR_LED_PIN, LOW);
        return reading;
    }
};

class Calculator {
private:
    float lastIRReading = 0.0;
    float lastRedReading = 0.0;
    bool isFingerDetected = false;

public:
    void updateReadings(float irReading, float redReading) {
        lastIRReading = irReading;
        lastRedReading = redReading;
        isFingerDetected = (irReading > SENSOR_THRESHOLD) && (redReading > SENSOR_THRESHOLD);
    }

    bool isReading() {
        return isFingerDetected;
    }

    float calculateLightLevelDifference() {
        return abs(lastRedReading - lastIRReading);
    }

    float bloodOxygenFormula() {
        if (!isFingerDetected) return 0;
        float R = (lastRedReading / lastIRReading);
        return -25 * R + 110;  // Example calibration equation
    }
};

class Displayer {
public:
    void print(const String& message) {
        Serial.println(message);
    }
};

// Instantiation of classes
DSP dsp;
Calculator calculator;
Displayer displayer;

void setup() {
    Serial.begin(9600);
    // Change to 14-bit resolution (Assuming using an Arduino Uno R4 Minima which has an ADC resolution of 14 bits)
    analogReadResolution(14); 
    pinMode(sensorPin, INPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(IR_LED_PIN, OUTPUT);
}

void loop() {
    // Read light levels
    float irReading = dsp.readLight(sensorPin);
    float redReading = dsp.readLight(sensorPin);

    // Update calculator readings
    calculator.updateReadings(irReading, redReading);

    // Check if a reading is possible
    if (calculator.isReading()) {
        float spo2 = calculator.bloodOxygenFormula();
        displayer.print("SpO2: " + String(spo2) + "%");
    } else {
        displayer.print("Please insert finger");
    }

    delay(1000); // Delay to reduce the reading speed
}