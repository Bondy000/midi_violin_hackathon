#include <Arduino.h>

#include <digitalWriteFast.h>

#define SOFTWARE_SERIAL_SPEED 9600

#define PI_MINIMUM_VALUE 3.14
#define PULSES_IN_TICK 360.0

#define MAX_ROTATION_SPEED 600

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 4

#define NOISE_VALUE_DECREASE 10

volatile int encoderTicks = 0; //Keeps track of encoder ticks
volatile unsigned long lastTickTime = 0; //Time of last encoder tick
volatile float rotationSpeed = 0; //Current rotation Speed in RPM

volatile int rotationDirection = 0;
volatile int maxSpeed = 0;

volatile int continousLeft = 0;
volatile int continousRight = 0;

const int touchPin = A0;
int softPot_val = 0;

void onEncoderTick() { 
    if(digitalRead(ENCODER_B_PIN) == HIGH){
        continousRight++;//Adding to check if there is a continoues value of the same direction
        continousLeft = 0; //Zero the wrong direction
        if(continousRight >= NOISE_VALUE_DECREASE){
            encoderTicks++; 
        }        
    }
    else {
        continousLeft++; //Adding to check if there is a continoues value of the same direction
        continousRight = 0; //Zero the wrong direction
        if(continousLeft >= NOISE_VALUE_DECREASE){
            encoderTicks--;
        }     
    }
        
}


void setup() {
    Serial.begin(SOFTWARE_SERIAL_SPEED);
    Serial.println("Setup Start...");

    pinMode(ENCODER_A_PIN, INPUT_PULLUP);
    pinMode(ENCODER_B_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), onEncoderTick, RISING);

    Serial.println("...Setup End");
}

void loop() {
    
    softPot_val = analogRead(touchPin);
    softPot_val = map(softPot_val, 0, 1023, 0, 600);
    if(softPot_val > 15){
        softPot_val = map(softPot_val, 15, 540, 0 ,127);
        Serial.println(softPot_val);
    }

    delay(50);
    
    
    if(millis() - lastTickTime >= 100){

        rotationSpeed = (encoderTicks / PULSES_IN_TICK) / ((millis() - lastTickTime) / 60000.0); //RPM = (ticks / Pulses between Ticks) /(time / 60000)

        if(rotationSpeed)
            rotationDirection = rotationSpeed / abs(rotationSpeed);
        else
            rotationDirection = 0;

        Serial.println("Rotations:");
        Serial.println(rotationDirection);
        Serial.println(round(rotationSpeed));
        
        maxSpeed = round(abs(rotationSpeed)) > MAX_ROTATION_SPEED? MAX_ROTATION_SPEED : round(abs(rotationSpeed));
        Serial.println(map(maxSpeed, 0, MAX_ROTATION_SPEED, 0, 127));

        lastTickTime = millis();
        encoderTicks = 0;
    }

}


