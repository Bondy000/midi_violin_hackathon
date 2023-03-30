#include <Arduino.h>

#include <digitalWriteFast.h>

#define SOFTWARE_SERIAL_SPEED 9600
#define TIMER_VALUE_ms 10

const int touchPin = A0;

int softPot_val = 0;

void setup() {
    Serial.begin(SOFTWARE_SERIAL_SPEED);
    Serial.println("hello");

    cli(); //Stop interrupts

    //Set timer2 interrupt at 2kHz (0.5ms T)
    TCCR2A = 0;// Set entire TCCR2A register to 0
    TCCR2B = 0;// Set entire TCCR2B register to 0
    TCNT2 = 0; //Initialize counter value to 0
    //Set compare match register for 1kHz increments
    OCR2A = 249; // = (16 *10^6) / (64 * 1000) -1  (must be <256 because of Timer0)

    //turn CTC mode
    TCCR1A |= (1 << WGM12);
    //set CS012 and CS00 bits for 64 prescalar
    TCCR1B |= (1 << CS01) | (1 << CS00);
    //enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);

    sei(); //Allow interrupts
}

void loop() {
    
    softPot_val = analogRead(touchPin);
    softPot_val = map(softPot_val, 0, 1023, 0, 600);
    if(softPot_val)
        Serial.println(softPot_val);
    delay(50);
    
}