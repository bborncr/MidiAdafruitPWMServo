#include <Wire.h>
#include "MIDIUSB.h"
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)

int MIDICHANNEL = 0xB0; //MIDI Control Change 1 (0xB0)

void setup()
{
  Serial.begin(115200);
  //while (!Serial) {
  // ; // wait for serial port to connect. Needed for native USB
  //}
  Serial.println("Ready...");

  pwm.begin();

  pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz updates

  // PWM LEDs
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);


  // Digital pins
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

}

void loop() {
  midiEventPacket_t rx;
  rx = MidiUSB.read();
  if (rx.header != 0 && rx.byte1 == MIDICHANNEL) {
    if (rx.byte2 < 16) { //CC 0-15 are for the servos

      Serial.println(rx.byte3);
      int val = map(rx.byte3, 0, 127, SERVOMIN, SERVOMAX);
      pwm.setPWM(rx.byte2, 0, val);
    }
    switch (rx.byte2) { // actions for CC 16-32 can be setup here
      case 16:
        {
          int val = map(rx.byte3, 0, 127, 0, 255);
          analogWrite(3, val);
          break;
        }
      case 17:
        {
          int val = map(rx.byte3, 0, 127, 0, 255);
          analogWrite(5, val);
          break;
        }
      case 18:
        {
          int val = map(rx.byte3, 0, 127, 0, 255);
          analogWrite(6, val);
          break;
        }
      case 19:
        {
          int val = map(rx.byte3, 0, 127, 0, 255);
          analogWrite(9, val);
          break;
        }
      case 20:
        {
          int val = map(rx.byte3, 0, 127, 0, 255);
          analogWrite(10, val);
          break;
        }
      case 21:
        {
          int val = map(rx.byte3, 0, 127, 0, 255);
          analogWrite(11, val);
          break;
        }
      case 22:
        {
          digitalWrite(13, rx.byte3);
          break;
        }
      case 23:
        digitalWrite(2, rx.byte3);
        break;
      case 24:
        digitalWrite(4, rx.byte3);
        break;
      case 25:
        digitalWrite(8, rx.byte3);
        break;
      case 26:
        digitalWrite(12, rx.byte3);
        break;
      default:
        break;
    }
  }
}

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
