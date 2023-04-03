# Violin MIDI Hackathon

This project uses an Arduino board to read input from a rotary encoder and a membrane potentiometer, and convert the data into MIDI messages that can be used to control a music synthesizer. The code is written in Arduino language, and uses the MIDI library for handling MIDI messages.

## Hardware Requirements

- Arduino uno board
- Rotary encoder (YUMO A6A2)
- Membrane potentiometer (SOFTPOT)

## Program Requirements
- Hairless MIDI<->Serial Bridge
- LoopMIDI (for Windows) or MIDI Audio Setup (for Mac)
- MIDI library for Arduino (https://github.com/FortySevenEffects/arduino_midi_library)

## Installation

1) Upload the code to the Arduino board using the Arduino IDE or any other compatible IDE.
2) Connect the rotary encoder and the membrane potentiometer to the Arduino board as follows:
  - Rotary encoder: connect pin A to digital pin 2, pin B to digital pin 4, and GND to GND.
  - Membrane potentiometer: connect the analog pin to pin A0, and GND to GND.
3) Download and install the Hairless MIDI<->Serial Bridge from the official website.(https://projectgus.github.io/hairless-midiserial/)
4) Create a virtual MIDI input device using LoopMIDI (for Windows) or MIDI Audio Setup (for Mac).
5) Open the Hairless MIDI<->Serial Bridge and configure it to use the correct serial port and MIDI input device.
6) Run the Hairless MIDI<->Serial Bridge.
7) In your music DAW, select the virtual MIDI input device created in step 4.


## Usage

The code will continuously read input from the rotary encoder and the membrane potentiometer, and send MIDI messages to the virtual MIDI input device created in step 4 of the installation process. The rotary encoder is used to control the the velocity and direction of the MIDI messages, while the membrane potentiometer is used to control the pitch bend of the  messages.

To use the code, simply connect the Arduino board to your computer using a USB cable, and run the Hairless MIDI<->Serial Bridge as explained in step 7 of the installation process.

