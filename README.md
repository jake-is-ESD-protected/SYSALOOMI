# SYSALOOMI
**SY**nthesizer-**SA**mpler-**LOO**per-**MI**dicontroller
### [Knowledge Pool](#dev-knowledge-pool) | [Shopping List](#shopping) | [Device Tree](#device-tree) | [Random Thoughts](#random-thoughts)

## DEV knowledge pool

* [similar project](https://github.com/otem/mr_touchy_teensy_polysynth_drummachine/blob/master/MrTouchy.ino)
* [LED drivers](https://github.com/FastLED/FastLED/wiki/Overview)
* Kristof says, **force sensitive resistors** (FSR) are best for percussive touch pads
* [Teensy smart LED](https://learn.sparkfun.com/tutorials/getting-started-with-the-smartled-shield-for-teensy)

### Examples
#### Dynamic timer interrupts
From [here](https://forum.arduino.cc/t/changing-timer-frequency-on-the-fly/106300/3) (pure arduino):
```c++
/* Code to pulse pin 3 with a modulated signal
* Can be used to drive an IR LED to keep a TSOP IR reciever happy
* This allows you to use a modulated reciever and a continious beam detector
* By Mike Cook Nov 2011 - Released under the Open Source licence
*/
 volatile byte pulse = 0;

ISR(TIMER2_COMPB_vect){  // Interrupt service routine to pulse the modulated pin 3
    pulse++;
  if(pulse >= 8) { // change number for number of modulation cycles in a pulse
    pulse =0;
    TCCR2A ^= _BV(COM2B1); // toggle pin 3 enable, turning the pin on and off
  }
}

void setIrModOutput(){  // sets pin 3 going at the IR modulation rate
  pinMode(3, OUTPUT);
  TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20); // Just enable output on Pin 3 and disable it on Pin 11
  TCCR2B = _BV(WGM22) | _BV(CS22);
  OCR2A = 51; // defines the frequency 51 = 38.4 KHz, 54 = 36.2 KHz, 58 = 34 KHz, 62 = 32 KHz
  OCR2B = 26;  // deines the duty cycle - Half the OCR2A value for 50%
  TCCR2B = TCCR2B & 0b00111000 | 0x2; // select a prescale value of 8:1 of the system clock
}

void setup(){
  setIrModOutput();
  TIMSK2 = _BV(OCIE2B); // Output Compare Match B Interrupt Enable
}

void loop(){
// do something here
}
```
Or [here](https://forum.arduino.cc/t/how-to-change-timer-interval-in-interrupt-routine/1044963/2)(for ESP32):
```c++
#define _TIMERINTERRUPT_LOGLEVEL_ 1

#include "ESP32_New_TimerInterrupt.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN       2         // Pin D2 mapped to pin GPIO2/ADC12 of ESP32, control on-board LED
#endif

#define TRK_PIN_OUTPUT 19

#define TRK_OnTime 10

#define TRK_OffTime 1000

// Init ESP32 timer 0
#define TIMER0_INTERVAL_MS 1000

ESP32Timer ITimer0(0);

// Interrupt routine
bool IRAM_ATTR TimerHandler0(void *timerNo)
{
  static bool toggle0 = false;
  digitalWrite(TRK_PIN_OUTPUT, toggle0);
  digitalWrite(LED_BUILTIN, toggle0);

  noInterrupts();

  // Set time to next flip according to current toggle0-state
  ITimer0.setInterval((toggle0 ? TRK_OnTime : TRK_OffTime) * 1000, TimerHandler0);

  interrupts();

  toggle0 = !toggle0;

  return true;
}

void setup()
{
  pinMode(TRK_PIN_OUTPUT, OUTPUT);
  pinMode(LED_BUILTIN,    OUTPUT);

  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  delay(100);

  Serial.print(F("\nStarting ChangeInterval_Crash on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP32_NEW_TIMERINTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Interval in microsecs
  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0);
}

void loop()
{
  delay(10);
}
```
[This](https://forum.arduino.cc/t/smoothly-changing-the-frequency-of-a-timer/218352) might also be helpful

## Shopping
* [Detentless Encoders](https://www.mouser.de/ProductDetail/Bourns/PEC12R-4025F-N0024?qs=Zq5ylnUbLm4HSBD7%2FFgU%2FA%3D%3D)
* [Touch pad](https://upverter.com/design/marcteys/e5bbec5797fd0064/muca-breakout---53-multitouch-test-panel/)
* [Piezo for pads](https://www.mouser.de/ProductDetail/SparkFun/SEN-09196?qs=WyAARYrbSnYalu6YbkDQBw%3D%3D)
* [Nintendo DS touch panel](https://www.ebay.de/itm/192537068207?hash=item2cd41a82af:g:RbUAAOSwmMNbxx~S)
* [Teensy](https://www.sparkfun.com/products/16771) or [Teensy](https://www.amazon.de/-/en/Teensy-4-1-without-pins/dp/B088D3FWR7/ref=sr_1_4?crid=1GU896OAP0O9K&keywords=teensy&qid=1681974253&sprefix=teensy%2Caps%2C125&sr=8-4)
* [Daisy seed](https://www.electro-smith.com/daisy/daisy)
* [RGB matrix high-end](https://www.sparkfun.com/products/14646)
* [RGB matrix cheap](https://www.amazon.de/-/en/dp/B079HVW652?th=1)

## Device Tree

> **Sync** processes run all the time and are mainly concerned with sampling something.  
**Async** processes get triggered by the user, compute and then die until reactivated.

|Sync processes|Async Processes|Misc|
|-|-|-|
|Adjustable metronome with subdivisions|Rotary Encoder based input|General filters|
|Quantizing Routine|Simple LED output|Linked list with f-pointers for arbitrary cascades|
|Buffer Playback|Simple Button Input|MIDI-Driver|
|Digital Mixing|CLI||
|Wave synthesis|Drumpad activation||
|SD-card/USB device reading|Touch pad interaction||
|UI animations|||

## Random Thoughts

### Hierarchy issues
Already I think that the splits are questionable; for example, the **looper** and **sampler** are pretty much the same, except that one sources from the ADC and the other from memory. In essence, the looper starts with a full section sample, which could be used in the sampler as well, but with a "blank" sample which the shorter samples are mixed onto. That essentially uses exactly the same tools except for the memory loading process. Therefore these modules could be merged. 

### Simplicity of MIDI-module
As long as no complex duplex data is required, MIDI signals are only triggered on async events or by some extent, sync clock pulses. This means that if a clear linearity between triggers and commands exists, a command can be processed ant sent immideatly in a very procedural way, making the MIDI-module more of a LUT than a complex model.