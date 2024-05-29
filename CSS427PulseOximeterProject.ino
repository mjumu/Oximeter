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
            
        }
        return result;
    }
};

class Calculator {
public:
    
    double bloodOxygenFormula(DSP dsp) {
        return log(dsp.readLight(sensorPin));  // Example calibration equation
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
    //analogReadResolution(14); 
    pinMode(sensorPin, INPUT);
}

void loop() {
    

    // Check if a reading is possible
    
    float spo2 = calculator.bloodOxygenFormula(dsp);
    if (active_state != 0) {
        displayer.print("SpO2: " + String(spo2) + "%");
        active_state = 0;
    }

    delay(1000); // Delay to reduce the reading speed
}
