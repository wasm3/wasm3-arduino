/*
 * Wasm3 - high performance WebAssembly interpreter written in C.
 * Copyright © 2020 Volodymyr Shymanskyy, Steven Massey.
 * All rights reserved.
 */

#include <wasm3.h>

/*
 * Configuration
 */

// Redefine the default LED pin here, if needed
#define LED_PIN             13

#define WASM_STACK_SLOTS    1024
#define NATIVE_STACK_SIZE   (32*1024)

// For (most) devices that cannot allocate a 64KiB wasm page
#define WASM_MEMORY_LIMIT   2048

/*
 * WebAssembly app
 *
 * This is essentially a simple "Blink" sketch, compiled to WebAssembly
 * You can build a wasm binary using C/C++, Rust, AssemblyScript, TinyGo, ...
 * See https://github.com/wasm3/wasm3-arduino/tree/master/wasm_apps for details
 */

unsigned char app_wasm[] = {
  0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00, 0x01, 0x11, 0x04, 0x60,
  0x02, 0x7f, 0x7f, 0x00, 0x60, 0x00, 0x00, 0x60, 0x01, 0x7f, 0x00, 0x60,
  0x00, 0x01, 0x7f, 0x02, 0x4e, 0x04, 0x07, 0x61, 0x72, 0x64, 0x75, 0x69,
  0x6e, 0x6f, 0x09, 0x67, 0x65, 0x74, 0x50, 0x69, 0x6e, 0x4c, 0x45, 0x44,
  0x00, 0x03, 0x07, 0x61, 0x72, 0x64, 0x75, 0x69, 0x6e, 0x6f, 0x07, 0x70,
  0x69, 0x6e, 0x4d, 0x6f, 0x64, 0x65, 0x00, 0x00, 0x07, 0x61, 0x72, 0x64,
  0x75, 0x69, 0x6e, 0x6f, 0x0c, 0x64, 0x69, 0x67, 0x69, 0x74, 0x61, 0x6c,
  0x57, 0x72, 0x69, 0x74, 0x65, 0x00, 0x00, 0x07, 0x61, 0x72, 0x64, 0x75,
  0x69, 0x6e, 0x6f, 0x05, 0x64, 0x65, 0x6c, 0x61, 0x79, 0x00, 0x02, 0x03,
  0x02, 0x01, 0x01, 0x05, 0x03, 0x01, 0x00, 0x01, 0x07, 0x13, 0x02, 0x06,
  0x6d, 0x65, 0x6d, 0x6f, 0x72, 0x79, 0x02, 0x00, 0x06, 0x5f, 0x73, 0x74,
  0x61, 0x72, 0x74, 0x00, 0x04, 0x0a, 0x3a, 0x01, 0x38, 0x01, 0x01, 0x7f,
  0x41, 0x80, 0x08, 0x10, 0x00, 0x22, 0x00, 0x36, 0x02, 0x00, 0x20, 0x00,
  0x41, 0x01, 0x10, 0x01, 0x03, 0x40, 0x41, 0x80, 0x08, 0x28, 0x02, 0x00,
  0x41, 0x01, 0x10, 0x02, 0x41, 0xe4, 0x00, 0x10, 0x03, 0x41, 0x80, 0x08,
  0x28, 0x02, 0x00, 0x41, 0x00, 0x10, 0x02, 0x41, 0x84, 0x07, 0x10, 0x03,
  0x0c, 0x00, 0x0b, 0x00, 0x0b
};
unsigned int app_wasm_len = 197;

/*
 * API bindings
 *
 * Note: each RawFunction should complete with one of these calls:
 *   m3ApiReturn(val)   - Returns a value
 *   m3ApiSuccess()     - Returns void (and no traps)
 *   m3ApiTrap(trap)    - Returns a trap
 */

#include <m3_api_defs.h>
#include <m3_env.h>

m3ApiRawFunction(m3_arduino_millis)
{
    m3ApiReturnType (uint32_t)

    m3ApiReturn(millis());
}

m3ApiRawFunction(m3_arduino_delay)
{
    m3ApiGetArg     (uint32_t, ms)

    // You can also trace API calls
    //Serial.print("api: delay "); Serial.println(ms);

    delay(ms);

    m3ApiSuccess();
}

// This maps pin modes from arduino_wasm_api.h
// to actual platform-specific values
uint8_t mapPinMode(uint8_t mode)
{
    switch(mode) {
    case 0: return INPUT;
    case 1: return OUTPUT;
    case 2: return INPUT_PULLUP;
    }
    return INPUT;
}

m3ApiRawFunction(m3_arduino_pinMode)
{
    m3ApiGetArg     (uint32_t, pin)
    m3ApiGetArg     (uint32_t, mode)

#if !defined(PARTICLE)
    typedef uint8_t PinMode;
#endif
    pinMode(pin, (PinMode)mapPinMode(mode));

    m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_digitalWrite)
{
    m3ApiGetArg     (uint32_t, pin)
    m3ApiGetArg     (uint32_t, value)

    digitalWrite(pin, value);

    m3ApiSuccess();
}

m3ApiRawFunction(m3_arduino_getPinLED)
{
    m3ApiReturnType (uint32_t)

    m3ApiReturn(LED_PIN);
}

m3ApiRawFunction(m3_arduino_print)
{
    m3ApiGetArgMem(const char *, out)
    m3ApiGetArg(int32_t, out_len)

    //printf("api: print %p\n", out);

    char buff[out_len+1];
    memcpy(buff, out, out_len);
    buff[out_len] = '\0';

    Serial.print(buff);

    m3ApiSuccess();
}

m3ApiRawFunction(m3_dummy)
{
    m3ApiSuccess();
}

M3Result  LinkArduino  (IM3Runtime runtime)
{
    IM3Module module = runtime->modules;
    const char* arduino = "arduino";

    m3_LinkRawFunction (module, arduino, "millis",           "i()",    &m3_arduino_millis);
    m3_LinkRawFunction (module, arduino, "delay",            "v(i)",   &m3_arduino_delay);
    m3_LinkRawFunction (module, arduino, "pinMode",          "v(ii)",  &m3_arduino_pinMode);
    m3_LinkRawFunction (module, arduino, "digitalWrite",     "v(ii)",  &m3_arduino_digitalWrite);

    // Convenience functions
    m3_LinkRawFunction (module, arduino, "print",            "v(*i)",  &m3_arduino_print);
    m3_LinkRawFunction (module, arduino, "getPinLED",        "i()",    &m3_arduino_getPinLED);

    // Dummy (for TinyGo)
    m3_LinkRawFunction (module, "env",   "io_get_stdout",    "i()",    &m3_dummy);

    return m3Err_none;
}

/*
 * Engine start, liftoff!
 */

#define FATAL(func, msg) { Serial.print("Fatal: " func " "); Serial.println(msg); return; }

void wasm_task(void*)
{
    M3Result result = m3Err_none;

    IM3Environment env = m3_NewEnvironment ();
    if (!env) FATAL("NewEnvironment", "failed");

    IM3Runtime runtime = m3_NewRuntime (env, WASM_STACK_SLOTS, NULL);
    if (!runtime) FATAL("NewRuntime", "failed");

#ifdef WASM_MEMORY_LIMIT
    runtime->memoryLimit = WASM_MEMORY_LIMIT;
#endif

    IM3Module module;
    result = m3_ParseModule (env, &module, app_wasm, app_wasm_len-1);
    if (result) FATAL("ParseModule", result);

    result = m3_LoadModule (runtime, module);
    if (result) FATAL("LoadModule", result);

    result = LinkArduino (runtime);
    if (result) FATAL("LinkArduino", result);

    IM3Function f;
    result = m3_FindFunction (&f, runtime, "_start");
    if (result) FATAL("FindFunction", result);

    Serial.println("Running WebAssembly...");

    const char* i_argv[1] = { NULL };
    result = m3_CallWithArgs (f, 0, i_argv);

    // Should not arrive here

    if (result) {
        M3ErrorInfo info;
        m3_GetErrorInfo (runtime, &info);
        Serial.print("Error: ");
        Serial.print(result);
        Serial.print(" (");
        Serial.print(info.message);
        Serial.println(")");
    }
}

void setup()
{
    Serial.begin(115200);
    delay(100);

    // Wait for serial port to connect
    // Needed for native USB port only
    while(!Serial) {}

    Serial.println("\nWasm3 v" M3_VERSION ", build " __DATE__ " " __TIME__);

#ifdef ESP32
    // On ESP32, we can launch in a separate thread
    xTaskCreate(&wasm_task, "wasm3", NATIVE_STACK_SIZE, NULL, 5, NULL);
#else
    wasm_task(NULL);
#endif
}

void loop()
{
    delay(100);
}

