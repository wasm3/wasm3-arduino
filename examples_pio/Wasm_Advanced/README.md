## PlatformIO example (advanced)

`./wasm_apps` contains sample apps in `C/C++`, `Rust`, `AssemblyScript`, `TinyGo`

`./wasm_vm` contains the host interpreter/VM.  
You can add your device type in `platformio.ini`.  
Adjust the LED pin number with `-DLED_PIN` option.

To run the example:
```sh
pio run -e <device> -t upload && pio device monitor
```
Where `<device>` is one of:  
`ESP32`, `ESP8266`, `Arduino101`, `MKR1000`, `NucleoWB55RG`, `BluePill`, `TinyBLE`, `Teensy31`, `Teensy40`, `WildFireV3`

**Note:** This example uses Wasm Linear Memory. You should be able to run it on any device that can afford to allocate 1-2 pages of Wasm Linear Memory (i.e. have >= 128KiB RAM).

