#include <Arduino.h>

#include <digitalWriteFast.h>

#define SOFTWARE_SERIAL_SPEED 9600

#define PI_MINIMUM_VALUE 3.14

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 4

#define NOISE_VALUE_DECREASE 10

volatile int encoderTicks = 0; //Keeps track of encoder ticks
volatile unsigned long lastTickTime = 0; //Time of last encoder tick
volatile float rotationSpeed = 0; //Current rotation Speed in RPM

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
        rotationSpeed = (encoderTicks / 120.0) / ((millis() - lastTickTime) /60000.0); //RPM = (ticks / 120) /(time / 60000)
        //Speed = Time Cycle * perimeter of the Encoder
        //Time Cycle = (Time counted) / Number of Ticks
        //Perimeter = 2 * pi * encoder raduis , pi = 3.14

        //rotationSpeed = ((millis() - lastTickTime) / (encoderTicks / 1000));// * (2.0 * PI_MINIMUM_VALUE * (2.0));
        Serial.println(rotationSpeed);
        lastTickTime = millis();
        encoderTicks = 0;
    }

}


