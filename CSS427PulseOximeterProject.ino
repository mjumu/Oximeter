#include <Arduino.h>
#include <LiquidCrystal.h>
// Assuming the necessary libraries for LCD and sensor handling are included
// #include <LiquidCrystal_I2C.h>

// Constants for pin assignments and thresholds
const int sensorPin = A0;
int resting_state; // No finger is present
int active_state; // Finger inbetween light and sensor
const int STATE_THRESHOLD = 750; // How we differentiate between resting and active states
LiquidCrystal LCD(2, 3, 4, 5, 6, 7);

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
        return log(dsp.readLight(sensorPin)) * 100;  // Example calibration equation
    }
};


  // LiquidCrystal LCD;

  // Displayer(int rs, int enable, int d4, int d5, int d6, int d7) {
  //   LiquidCrystal LCD(rs, enable, d4, d5, d6, d7);
  //   LCD.begin(16, 2);
  // }

  bool firstRow(String text) {
    LCD.setCursor(0, 0);
    LCD.print(text);
  }

  bool secondRow(String text) {
    LCD.setCursor(0, 1);
    LCD.print(text);
  }


// Instantiation of classes
DSP dsp;
Calculator calculator;

void setup() {
    Serial.begin(9600);
    // Change to 14-bit resolution (Assuming using an Arduino Uno R4 Minima which has an ADC resolution of 14 bits)
    //analogReadResolution(14); 
    LiquidCrystal LCD(2, 3, 4, 5, 6, 7);
    LCD.begin(16, 2);
    pinMode(sensorPin, INPUT);
}

void loop() {
    

    // Check if a reading is possible
    
    float spo2 = calculator.bloodOxygenFormula(dsp);
    if (active_state != 0) {
        firstRow("SpO2: " + String(spo2) + "%");
        active_state = 0;
    } else {
      LCD.clear();
    }

    delay(1000); // Delay to reduce the reading speed
}
