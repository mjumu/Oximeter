#include <Arduino.h>

// Assuming the necessary libraries for LCD and sensor handling are included
// #include <LiquidCrystal_I2C.h>

// Constants for pin assignments and thresholds
const int sensorPin = A0;
int resting_state; // No finger is present
int active_state; // Finger inbetween light and sensor
const int STATE_THRESHOLD = 750; // How we differentiate between resting and active states

// Class Definitions
class DSP {
public:
    double readLight(int pin) {
        int read = analogRead(pin);
        if (read >= STATE_THRESHOLD) {
            resting_state = read;
        }else {
            active_state = read;
        }
        double result = 0.0;
        if (active_state != 0) {
            result = (double) (resting_state/active_state);
            active_state = 0;
        }
        return result;
    }
};

class Calculator {
public:
    
    double bloodOxygenFormula() {
        return log(DSP.readLight(sensorPin));  // Example calibration equation
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
