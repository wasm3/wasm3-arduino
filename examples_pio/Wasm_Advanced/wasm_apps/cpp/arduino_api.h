#ifndef arduino_wasm_api_h
#define arduino_wasm_api_h

#include <stdint.h>

#define WASM_EXPORT                   extern "C" __attribute__((used)) __attribute__((visibility ("default")))
#define WASM_EXPORT_AS(NAME)          WASM_EXPORT __attribute__((export_name(NAME)))
#define WASM_IMPORT(MODULE,NAME)      __attribute__((import_module(MODULE))) __attribute__((import_name(NAME)))
#define WASM_CONSTRUCTOR              __attribute__((constructor))

#define LOW               0x0
#define HIGH              0x1

#define INPUT             0x0
#define OUTPUT            0x1
#define INPUT_PULLUP      0x2

extern "C" {

WASM_IMPORT("arduino", "millis")          uint32_t millis     (void);
WASM_IMPORT("arduino", "delay")           void delay          (uint32_t ms);
WASM_IMPORT("arduino", "pinMode")         void pinMode        (uint32_t pin, uint32_t mode);
WASM_IMPORT("arduino", "digitalWrite")    void digitalWrite   (uint32_t pin, uint32_t value);

// This is a convenience function
WASM_IMPORT("arduino", "getPinLED")       uint32_t getPinLED  (void);
WASM_IMPORT("arduino", "getGreeting")     void getGreeting    (char* buff, size_t maxlen);
WASM_IMPORT("arduino", "print")           void print          (const char* s, size_t len);

static inline size_t strlen(const char *str) {
  const char *s;
  for (s = str; *s; ++s);
  return (s - str);
}

static inline void print (const char* s) {
  print(s, strlen(s));
}

static inline void println (const char* s) {
  print(s); print("\n");
}

}

#endif // arduino_wasm_api_h
