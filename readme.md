# Teensy MIDI Controller with LED Feedback

## Overview

This project implements a MIDI controller using a Teensy microcontroller, featuring 10 buttons and 10 analog potentiometers. The controller sends MIDI Note On and Note Off messages based on button presses/releases and potentiometer adjustments. Additionally, it includes a visual LED feedback system using WS281X LEDs.

## Features

- **10 Buttons**: Each button can trigger a MIDI note.
- **10 Potentiometers**: Adjustments send MIDI messages with the potentiometer values.
- **LED Feedback**: WS2812X LEDs display colors based on MIDI note actions.
- **LED Startup Animation**: A visual startup sequence for the LED strip.
- **Debouncing**: Implemented to ensure accurate button presses.

## Usage

**Please consider buying my Teensy-breakout-board on Tindie.**
<a href="https://www.tindie.com/stores/etech/?ref=offsite_badges&utm_source=sellers_jannik_de&utm_medium=badges&utm_campaign=badge_medium"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-mediums.png" alt="I sell on Tindie" width="150" height="78"></a>

- **Button Press**: Press any of the 10 buttons to send a MIDI Note On message. The corresponding LED will light up based on the MIDI note value.
- **Button Release**: Release the button to send a MIDI Note Off message.
- **Potentiometer Adjustment**: Turn any of the 10 potentiometers to send MIDI Note On messages with values corresponding to the potentiometer position (0-127).

## LED Control

The LED colors and brightness are controlled based on the MIDI messages received. Each LED will light up according to the following color encoding:

- **Velocity**: Controls the brightness (0-127).
- **Color**: Encoded in the lower 6 bits of the velocity value.

## Notes

- Adjust the `brightness_dimm` and `brightness_full` constants in the code to modify LED brightness levels.
- Adjust the USB-name in the name.c

## Troubleshooting

- If the LEDs do not light up, check the power supply to the LED strip.
- Ensure all connections are secure and correctly wired.
- Use a multimeter to verify button functionality and potentiometer readings.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.

## Acknowledgments

- Thanks to the Teensy community for their excellent resources and support.
- Special thanks to the authors of the FastLED and Bounce2 libraries.
