#include <Arduino.h>

#include <digitalWriteFast.h>

#define SOFTWARE_SERIAL_SPEED 9600
#define TIMER_VALUE_us 500

#define encoderA 5
#define encoderB 4

const int touchPin = A0;
int softPot_val = 0;


int timerFlag = 0;
unsigned long milliTime = 0;
unsigned long pulseHappen = 0;

int interruptEncoder = 2; //define interrupt pin to 2
volatile int state = LOW; // To make sure variables shared between an ISR

void encoderInterrupt() { 
    if(!timerFlag){
        milliTime = micros();
        timerFlag = 1;
    }
    
    if(timerFlag)
        pulseHappen++;
}

void setup() {
    Serial.begin(SOFTWARE_SERIAL_SPEED);
    Serial.println("Setup Start...");
    
    pinMode(interruptEncoder, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(interruptEncoder), encoderInterrupt, RISING);
    
    
    /**

    //Set timer2 interrupt at 2kHz (0.5ms T)
    TCCR2A = 0;// Set entire TCCR2A register to 0
    TCCR2B = 0;// Set entire TCCR2B register to 0
    TCNT2 = 0; //Initialize counter value to 0
    //Set compare match register for 1kHz increments
    OCR2A = 124; // = (16 *10^6) / (64 * 2000) -1  (must be <256 because of Timer2)

    //turn CTC mode
    //TCCR2A |= (1 << WGM21);
    //set CS21 and CS20 bits for 64 prescalar
    TCCR2B |= (1 << CS21) | (1 << CS20);

    //enable timer compare interrupt
    TIMSK2 |= (1 << OCIE2A);
*/
    sei(); //Allow interrupts
}

void loop() {
    
    softPot_val = analogRead(touchPin);
    softPot_val = map(softPot_val, 0, 1023, 0, 600);
    if(softPot_val > 15){
        softPot_val = map(softPot_val, 15, 540, 0 ,127);
        Serial.println(softPot_val);
    }

    delay(50);
    
    if(timerFlag){
        unsigned long currentTime = micros();
        if(milliTime > currentTime){
            currentTime = (unsigned long)(0 - milliTime) + currentTime;
        } else{
            currentTime = currentTime - milliTime;
        }

        if(currentTime >= 500){
            Serial.println("Pulse is:");
            Serial.println(pulseHappen);
            pulseHappen = 0;
            timerFlag = 0;

            delay(50);
        }
    }

    /*
    if(digitalReadFast(encoderA)){
        if(!timerFlag){
            timerFlag = 1;  
            milliTime = millis();
        } 
        else {
            pulseHappen++;
            if(millis() - milliTime >= 1000){
                
                Serial.println(pulseHappen);
                pulseHappen = 0;
                timerFlag = 0;
            }
        }

        if(digitalRead(encoderB)){

        }
    }
*/
    //delay(50);

}


