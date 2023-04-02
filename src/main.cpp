#include <Arduino.h>
#include <digitalWriteFast.h>
#include <MIDI.h>

//violin midi hackathon


#define SOFTWARE_SERIAL_SPEED 115200

#define PI_MINIMUM_VALUE 3.14
#define PULSES_IN_TICK 360.0

#define MAX_ROTATION_SPEED 600

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 4

#define NOISE_VALUE_DECREASE 10

MIDI_CREATE_DEFAULT_INSTANCE();

volatile int encoderTicks = 0;            //Keeps track of encoder ticks
volatile unsigned long lastTickTime = 0;  //Time of last encoder tick
volatile float rotationSpeed = 0;         //Current rotation Speed in RPM

volatile int rotationDirection = 0;
volatile int maxSpeed = 0;

volatile int continousLeft = 0;
volatile int continousRight = 0;
int isOn = 0;
int isTouced = 0;

const int touchPin = A0;
int softPot_val = 0;
byte midiCh = 1;    //* MIDI channel to be used
byte midiCh2 = 10;  //* MIDI channel to be used

byte note = 64;  //* Lowest note to be used
byte cc = 20;    //* Lowest MIDI CC to be usedb
byte velocity = 127;
int lastDir = 0;
void onEncoderTick() {
  if (digitalRead(ENCODER_B_PIN) == HIGH) {
    continousRight++;   //Adding to check if there is a continoues value of the same direction
    continousLeft = 0;  //Zero the wrong direction
    if (continousRight >= NOISE_VALUE_DECREASE) {
      encoderTicks++;
    }
  } else {
    continousLeft++;     //Adding to check if there is a continoues value of the same direction
    continousRight = 0;  //Zero the wrong direction
    if (continousLeft >= NOISE_VALUE_DECREASE) {
      encoderTicks--;
    }
  }
}
void flipDirection() {
  MIDI.sendNoteOff(note, velocity, midiCh);
  MIDI.sendNoteOn(note, velocity, midiCh);
}

void setup() {
  Serial.begin(SOFTWARE_SERIAL_SPEED);
  ////Serial.println("Setup Start...");

  pinMode(ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(ENCODER_B_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN), onEncoderTick, RISING);

  ////Serial.println("...Setup End");
}

void loop() {

  softPot_val = analogRead(touchPin);

  //softPot_val = map(softPot_val, 0, 1023, 0, 16384);
  if (softPot_val > 25) {
    softPot_val = map(softPot_val, 25, 1023, 0, 8192);
    //Serial.println("\n");

    // Serial.println(softPot_val);

    MIDI.sendPitchBend(softPot_val, midiCh);
    isTouced = 1;

    //Serial.println(softPot_val);
  } else {
    isTouced = 0;
  }



  if (millis() - lastTickTime >= 100) {

    rotationSpeed = (encoderTicks / PULSES_IN_TICK) / ((millis() - lastTickTime) / 60000.0);  //RPM = (ticks / Pulses between Ticks) /(time / 60000)

    if (rotationSpeed)
      rotationDirection = rotationSpeed / abs(rotationSpeed);
    else
      rotationDirection = 0;

    if (rotationDirection != lastDir) {
      switch (rotationDirection) {
        case -1:
          flipDirection();
          isOn = 1;

          break;
        case 0:
          MIDI.sendNoteOff(note, velocity, midiCh);
          isOn = 0;
          break;
        case 1:
          isOn = 1;

          flipDirection();
          break;
      }
    }
    //Serial.println("Rotations:");
    //Serial.println(rotationDirection);
    //Serial.println(round(rotationSpeed));

    maxSpeed = round(abs(rotationSpeed)) > MAX_ROTATION_SPEED ? MAX_ROTATION_SPEED : round(abs(rotationSpeed));
    int tempVal = map(maxSpeed, 0, MAX_ROTATION_SPEED, 0, 127);
    //Serial.println(tempVal);
    if (tempVal && isOn)
      MIDI.sendControlChange(cc, tempVal, midiCh);

    //Serial.println();

    lastTickTime = millis();
    encoderTicks = 0;
    lastDir = rotationDirection;
  }
  //
  delay(15);
}

