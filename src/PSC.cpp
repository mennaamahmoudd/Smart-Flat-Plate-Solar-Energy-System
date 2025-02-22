#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include "max6675.h"
#include <SPI.h>
    const int rs = 12, en = 13, d4 = 8, d5 = 7, d6 = 2, d7 = 1;
    LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

    int soPin = 4;  // SO = Serial Out
    int csPin = 5;  // CS = chip select CS pin
    int sckPin = 6; // SCK = Serial Clock pin
    MAX6675 Module(sckPin, csPin, soPin); // create instance object of MAX6675

    Servo tower1Servo_X;
    Servo tower1Servo_Y;
    Servo tower2Servo_X;
    Servo tower2Servo_Y;

    const int LEFT = 120;
    const int RIGHT = 60;
    const int STOP = 90;

    const int ldr1TopRight = A0;
    const int ldr1BottomRight = A1;
    const int ldr1BottomLeft = A2;
    const int ldr1TopLeft = A3;
    const int ldr2TopRight = A4;
    const int ldr2BottomRight = A5;
    const int ldr2BottomLeft = A6;
    const int ldr2TopLeft = A7;

    float temp = 0;

    void setup() {
        lcd.begin(16, 2);
        lcd.print("Temperature");

        tower1Servo_Y.attach(3);
        tower1Servo_X.attach(9);
        tower2Servo_Y.attach(10);
        tower2Servo_X.attach(11);
    }



void loop() {
    getTemp();

    int ldr1TopRightStatus = analogRead(ldr1TopRight);
    int ldr1BottomRightStatus = analogRead(ldr1BottomRight);
    int ldr1BottomLeftStatus = analogRead(ldr1BottomLeft);
    int ldr1TopLeftStatus = analogRead(ldr1TopLeft);
    int ldr2TopRightStatus = analogRead(ldr2TopRight);
    int ldr2BottomRightStatus = analogRead(ldr2BottomRight);
    int ldr2BottomLeftStatus = analogRead(ldr2BottomLeft);
    int ldr2TopLeftStatus = analogRead(ldr2TopLeft);

    int topAvgTower1 = (ldr1TopRightStatus + ldr1TopLeftStatus) / 2;
    int rightAvgTower1 = (ldr1TopRightStatus + ldr1BottomRightStatus) / 2;
    int bottomAvgTower1 = (ldr1BottomRightStatus + ldr1BottomLeftStatus) / 2;
    int leftAvgTower1 = (ldr1BottomLeftStatus + ldr1TopLeftStatus) / 2;

    int topAvgTower2 = (ldr2TopRightStatus + ldr2TopLeftStatus) / 2;
    int rightAvgTower2 = (ldr2TopRightStatus + ldr2BottomRightStatus) / 2;
    int bottomAvgTower2 = (ldr2BottomRightStatus + ldr2BottomLeftStatus) / 2;
    int leftAvgTower2 = (ldr2BottomLeftStatus + ldr2TopLeftStatus) / 2;

    // Tower one
    if (ldr1TopRightStatus > 300 || ldr1BottomRightStatus > 300 || ldr1BottomLeftStatus > 300 || ldr1TopLeftStatus < 300) {
        if (topAvgTower1 > bottomAvgTower1 && (topAvgTower1 - bottomAvgTower1 > 30)) moveTower1Down();
        else if (bottomAvgTower1 > topAvgTower1 && (bottomAvgTower1 - topAvgTower1 > 30)) moveTower1Up();

        if (rightAvgTower1 > leftAvgTower1 && (rightAvgTower1 - leftAvgTower1 > 15)) moveTower1Left();
        else if (leftAvgTower1 > rightAvgTower1 && (leftAvgTower1 - rightAvgTower1 > 15)) moveTower1Right();
    }

    // Tower two
    if (ldr2TopRightStatus > 300 || ldr2BottomRightStatus > 300 || ldr2BottomLeftStatus > 300 || ldr2TopLeftStatus < 300) {
        if (topAvgTower2 > bottomAvgTower2 && (topAvgTower2 - bottomAvgTower2 > 15)) moveTower2Down();
        else if (bottomAvgTower2 > topAvgTower2 && (bottomAvgTower2 - topAvgTower2 > 15)) moveTower2Up();

        if (rightAvgTower2 > leftAvgTower2 && (rightAvgTower2 - leftAvgTower2 > 15)) moveTower2Left();
        else if (leftAvgTower2 > rightAvgTower2 && (leftAvgTower2 - rightAvgTower2 > 15)) moveTower2Right();
    }

    printTempOnLcd();
}

void moveTower2Down() {
    tower2Servo_Y.write(RIGHT);
    delay(10);
    tower2Servo_Y.write(STOP);
    delay(10);
}

void moveTower2Up() {
    tower2Servo_Y.write(LEFT);
    delay(10);
    tower2Servo_Y.write(STOP);
    delay(10);
}

void moveTower2Right() {
    tower2Servo_X.write(RIGHT);
    delay(10);
    tower2Servo_X.write(STOP);
    delay(10);
}

void moveTower2Left() {
    tower2Servo_X.write(LEFT);
    delay(10);
    tower2Servo_X.write(STOP);
    delay(10);
}

void moveTower1Down() {
    tower1Servo_Y.write(RIGHT);
    delay(10);
    tower1Servo_Y.write(STOP);
    delay(10);
}

void moveTower1Up() {
    tower1Servo_Y.write(LEFT);
    delay(10);
    tower1Servo_Y.write(STOP);
    delay(10);
}

void moveTower1Right() {
    tower1Servo_X.write(RIGHT);
    delay(10);
    tower1Servo_X.write(STOP);
    delay(10);
}

void moveTower1Left() {
    tower1Servo_X.write(LEFT);
    delay(10);
    tower1Servo_X.write(STOP);
    delay(10);
}

void printTempOnLcd() {
    lcd.setCursor(0, 1);
    lcd.print(String(temp));
    lcd.print(" C");
}

unsigned long previousMillis = 0;
const long interval = 1000;

void getTemp() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        temp = Module.readCelsius();
    }
}