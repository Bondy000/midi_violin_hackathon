#include <Arduino.h>

#include <digitalWriteFast.h>

#define SOFTWARE_SERIAL_SPEED 9600
#define TIMER_VALUE_us 500


const int touchPin = A0;

int softPot_val = 0;

int timerFlag = 0;

void setup() {
    Serial.begin(SOFTWARE_SERIAL_SPEED);
    Serial.println("hello");

    /*cli(); //Stop interrupts

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

    sei(); //Allow interrupts*/
}

void loop() {
    
    softPot_val = analogRead(touchPin);
    softPot_val = map(softPot_val, 0, 1023, 0, 600);
    if(softPot_val > 15){
        softPot_val = map(softPot_val, 15, 540, 0 ,127);
        Serial.println(softPot_val);
    }
    
    delay(50);
    
}

/*ISR (TIMER2_COMPA_vect){
    timerFlag = 1;
}*/