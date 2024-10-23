#include <Bounce.h>
#include <FastLED.h>
#include "pins.h"

#define NUM_LEDS 10
CRGB leds[NUM_LEDS];

// Brightness levels
const uint8_t brightness_dimm = 30;
const uint8_t brightness_full = 255;

// Button debouncing
Bounce buttons[] = { 
  Bounce(BUTTON1, 5),
  Bounce(BUTTON2, 5),
  Bounce(BUTTON3, 5), 
  Bounce(BUTTON4, 5),
  Bounce(BUTTON5, 5),
  Bounce(BUTTON6, 5), 
  Bounce(BUTTON7, 5),
  Bounce(BUTTON8, 5),
  Bounce(BUTTON9, 5),
  Bounce(BUTTON10, 5)
};

// Previous state of analog values for change detection
int old_analog_vals[10] = {0};

// Function prototypes
void setupLEDs();
void setupButtons();
void checkButtons();
void handleAnalogInputs();
void controlLED(uint8_t led_number, char glow, uint8_t color);
void ledStartup();
void myNoteOn(byte midi_channel, byte note, byte velocity);
void myNoteOff(byte midi_channel, byte note, byte velocity);

int main() {
  setupLEDs();
  setupButtons();
  ledStartup();

  // Set up MIDI handlers
  usbMIDI.setHandleNoteOn(myNoteOn);
  usbMIDI.setHandleNoteOff(myNoteOff);

  while (1) {
    usbMIDI.read();  // Read incoming MIDI messages

    checkButtons();  // Check for button presses/releases
    handleAnalogInputs();  // Handle potentiometer input
  }
}

/* Function to initialize the LEDs */
void setupLEDs() {
  FastLED.addLeds<WS2812, DATA_PIN1, RGB>(leds, NUM_LEDS);
  pinMode(PCB_LED2, OUTPUT);
  pinMode(PCB_LED3, OUTPUT);
}

/* Function to initialize button pins */
void setupButtons() {
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(buttons[i].pin(), INPUT);  // Set button pins as input with pullup
  }
}

/* Function to check the button states */
void checkButtons() {
  for (int i = 0; i < NUM_LEDS; i++) {
    buttons[i].update();  // Update the state of each button

    // Trigger MIDI note on when button is pressed
    if (buttons[i].risingEdge()) {
      usbMIDI.sendNoteOn(i + 1, 127, midi_channel);
    }

    // Trigger MIDI note off when button is released
    if (buttons[i].fallingEdge()) {
      usbMIDI.sendNoteOff(i + 1, 0, midi_channel);
    }
  }
}

/* Function to handle analog input from potentiometers */
void handleAnalogInputs() {
  const int potPins[] = {POTI0, POTI1, POTI2, POTI3, POTI4, POTI5, POTI6, POTI7, POTI8, POTI9};
  int new_analog_vals[10];

  for (int i = 0; i < 10; i++) {
    new_analog_vals[i] = map(analogRead(potPins[i]), 0, 1023, 0, 127);
    
    if (abs(new_analog_vals[i] - old_analog_vals[i]) >= 2) {  // Only send when change is significant
      usbMIDI.sendNoteOn(i + 1, new_analog_vals[i], midi_channel);
      old_analog_vals[i] = new_analog_vals[i];  // Update the old value
    }
  }
}

/* Function to control LED color and brightness */
void controlLED(uint8_t led_number, char glow, uint8_t color) {
  long r = 85 * (((color >> 1) & 2) | ((color >> 5) & 1));
  long g = 85 * ((color & 2) | ((color >> 4) & 1));
  long b = 85 * (((color << 1) & 2) | ((color >> 3) & 1));

  if (glow) {
    r = (r * (brightness_dimm + 1)) >> 8;
    g = (g * (brightness_dimm + 1)) >> 8;
    b = (b * (brightness_dimm + 1)) >> 8;
  } else {
    r = (r * (brightness_full + 1)) >> 8;
    g = (g * (brightness_full + 1)) >> 8;
    b = (b * (brightness_full + 1)) >> 8;
  }

  leds[led_number].setRGB(r, g, b);
  FastLED.show();
}

/* Function for startup LED animation */
void ledStartup() {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = ColorFromPalette(RainbowColors_p, i * 32, 255, LINEARBLEND);
    FastLED.show();
    delay(500);
  }

  // Clear all LEDs after the startup animation
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::Black;
    FastLED.show();
  }
}

/* MIDI Note On handler */
void myNoteOn(byte midi_channel, byte note, byte velocity) {
  uint8_t nte = note & 0xF;
  char glw = velocity & 0x40;
  uint8_t clr = velocity & 0x3F;
  controlLED(nte, glw, clr);
}

/* MIDI Note Off handler */
void myNoteOff(byte midi_channel, byte note, byte velocity) {
  uint8_t nte = note & 0xF;
  char glw = velocity & 0x40;
  uint8_t clr = velocity & 0x3F;
  controlLED(nte, glw, clr);
}