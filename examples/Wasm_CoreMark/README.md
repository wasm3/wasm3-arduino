## CoreMark 1.0

### RAM requirements

- Wasm binary: `7.6 Kb`
- Wasm3 dynamic structures: `~34Kb`
  - Linear Memory: `3 Kb`
  - Wasm Stack: `2 Kb`
- Native stack: `~2 Kb`
- **Total: `~44Kb`**

Note: if no TCO is available, native stack can get much larger.  
I.e. it grows to `48 Kb` for ESP32. And this causes ESP8266 to hang.  
This will be improved in future. 🦄

### Results for some boards
```log
 Board                 |  MCU          |  Freq     |  CoreMark
-----------------------------------------------------------------
Teensy 4.0               iMXRT1062         1 GHz     109.842
Teensy 4.0               iMXRT1062       816 MHz      88.925
Teensy 4.0               iMXRT1062       600 MHz      65.383
Sipeed MAIX BiT          Kendryte K210   400 MHz      60.730
Adafruit pyBadge         ATSAMD51J19     200 MHz      15.315
Adafruit pyBadge         ATSAMD51J19     120 MHz       9.193
ESP32                    ESP32           240 MHz       8.778
ESP8266                  ESP8266          80 MHz     (hangs)
Teensy 3.2 fastest+lto   MK20DX256       120 MHz       6.800
BLE Nano 2               nRF52832         64 MHz       3.938
BBC micro:bit v2         nRF52833         64 MHz       3.908
```

