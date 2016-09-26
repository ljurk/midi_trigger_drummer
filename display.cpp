#include "display.h"
#include "midi.h"
#include <Arduino.h>

namespace Display {
  uint8_t characters[16] = {
      0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001, 0b10010010,
      0b10000010, 0b11111000, 0b10000000, 0b10010000, 0b10100000, 0b10000011,
      0b10100111, 0b10100001, 0b10000100, 0b10001110};

  void setup() {
    pinMode(DISPLAY_DATA, OUTPUT);
    pinMode(DISPLAY_CLK, OUTPUT);
    pinMode(DISPLAY_LATCH1, OUTPUT);
    pinMode(DISPLAY_LATCH2, OUTPUT);

    digitalWrite(DISPLAY_LATCH1, HIGH);
    digitalWrite(DISPLAY_LATCH2, HIGH);
  }

  void display(
      uint8_t character1, uint8_t character2, bool decimal1, bool decimal2,
      bool leading_zero) {
    uint8_t data1 = ! leading_zero && character1 == 0 ?
        0b11111111 :
        characters[character1];

    digitalWrite(DISPLAY_LATCH1, LOW);
    shiftOut(
        DISPLAY_DATA, DISPLAY_CLK, MSBFIRST, data1 - 0b10000000 * decimal1);
    digitalWrite(DISPLAY_LATCH1, HIGH);

    digitalWrite(DISPLAY_LATCH2, LOW);
    shiftOut(
        DISPLAY_DATA, DISPLAY_CLK, MSBFIRST,
        characters[character2] - 0b10000000 * decimal2);
    digitalWrite(DISPLAY_LATCH2, HIGH);
  }

  void displayHex(
      uint8_t character, bool decimal1, bool decimal2, bool leading_zero) {
    display(
        (character & 0xf0) >> 4,
        character & 0x0f,
        decimal1,
        decimal2,
        leading_zero);
  }

  void displayDec(
      uint8_t character, bool decimal1, bool decimal2, bool leading_zero) {
    display(
        character / 10,
        character % 10,
        decimal1,
        decimal2,
        leading_zero);
  }

  void displayMidiChannel(bool edit) {
    displayDec(MIDI::channel + 1, edit, false, false);
  }
}
