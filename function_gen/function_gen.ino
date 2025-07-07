#include "AD9833.h"
#include "LiquidCrystal_I2C.h"
#include "Rotary.h"
#include "Button2.h"

// AD9833 pins
#define FSYNC 9
#define SDATA 11
#define SCLK 13

// Rotary encoder pins
#define BTNPIN 4
#define DTPIN 3
#define CLKPIN 2

#define CLICKS_PER_STEP 4



String wave[5] = {"OFF", "SINE", "SQUARE1", "SQUARE2", "TRIANGLE"};
int waveSel = 2;
String cngType[4] = {"HIGH", "FINE", "LOW", "PRECISE"};
float cngAmt[4] = {10, 1, 0.1, 0.01};
int cngTypeSel = 0;
float frequency = 1000.0f;
int aState;
int aLastState;

int lastRotaryPosition = 0;

// Time tracking for displaying change type
unsigned long lastChangeTime = 0;
bool showChangeType = false;
int lastChangeType = 0; // Stores the last change type to display

AD9833 AD(FSYNC, SDATA, SCLK);
Rotary r;
Button2 b;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Make sure you have the correct LiquidCrystal_I2C library installed (by Frank de Brabander)

void updateScreen(bool downLine, int typePress) {
    lcd.clear();
    if (!downLine) {
        lcd.setCursor(0, 0);
        lcd.print("FREQ ");

        float bifreq = frequency;
        String unit = "Hz";

        if (bifreq >= 1000000) {
            bifreq /= 1000000;
            unit = "MHz";
        } else if (bifreq >= 1000) {
            bifreq /= 1000;
            unit = "KHz";
        }

        lcd.print(bifreq, 3);  // Print float with 3 decimal places
        lcd.print(" ");
        lcd.print(unit);
        lcd.setCursor(0,1);
        lcd.print("WAVTYPE " + wave[waveSel]);
    } else {
        lcd.setCursor(0, 1);
        switch (typePress) {
            case 0:
                lcd.print("WAVTYPE " + wave[waveSel]);
                break;
            case 1:
                lcd.print("CNTYPE " + cngType[cngTypeSel]);
                break;
            case 2:
                lcd.print("RESETTING...");
                delay(1000);
                lcd.clear();
                lcd.print("FREQ 001.000 KHz");
                break;
        }
        // Set flag to show change type for 1 second
        lastChangeTime = millis();
        showChangeType = true;
        lastChangeType = typePress;
    }
}

float getFreqMul(bool inc) {
    if (inc) {
        if (frequency < 100) return 1;
        else if (frequency < 1000) return 10;
        else if (frequency < 10000) return 100;
        else if (frequency < 100000) return 1000;
        else if (frequency < 1000000) return 10000;
        else if (frequency < 12000000) return 100000;
        else return 0;
    } else {
        if (frequency <= 0.1) return 0;
        else if (frequency < 1) return 0.001;
        else if (frequency < 10) return 0.01;
        else if (frequency < 100) return 0.1;
        else if (frequency < 1000) return 1;
        else if (frequency < 10000) return 10;
        else if (frequency < 100000) return 100;
        else if (frequency < 1000000) return 1000;
        else if (frequency < 10000000) return 10000;
        else return 100000;
    }
}

void btnPressHandler(Button2& btn) {
    switch (btn.getType()) {
        case single_click:
            waveSel = (waveSel + 1) % 5;
            updateScreen(true, 0);
            break;
        case double_click:
            cngTypeSel = (cngTypeSel + 1) % 4;
            updateScreen(true, 1);
            break;
        case long_click:
            waveSel = 2;
            cngTypeSel = 0;
            frequency = 1000.0f;
            updateScreen(true, 2);
            break;
    }
}

void rotaryHandler(Rotary& rotary) {
    int currentPos = rotary.getPosition();
    int dir = currentPos > lastRotaryPosition ? 1 : -1;

    if (currentPos != lastRotaryPosition) {
        frequency += dir * cngAmt[cngTypeSel] * getFreqMul(currentPos > lastRotaryPosition);
        updateScreen(false, 0);
    }
    lastRotaryPosition = currentPos;
}


void setup() {
  AD.begin();
  lcd.init();
  lcd.backlight();
  updateScreen(false,0);
  r.begin(CLKPIN,DTPIN, CLICKS_PER_STEP);

  r.setChangedHandler(rotaryHandler);

  b.begin(BTNPIN);
  b.setClickHandler(btnPressHandler);
  //button.setLongClickHandler(handler);       // this will only be called upon release
  b.setLongClickDetectedHandler(btnPressHandler);  // this will only be called upon detection
  b.setDoubleClickHandler(btnPressHandler);
}


    

void loop() {
    r.loop();
    b.loop();
    AD.setWave(waveSel);
    AD.setFrequency(frequency);
    AD.setFrequencyChannel(0);

    // Check if we need to switch back to frequency display after 1 second
    if (showChangeType && millis() - lastChangeTime > 1000) {
        showChangeType = false;
        updateScreen(false, 0); // Revert back to frequency display
    }
}