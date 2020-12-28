/*
 * Wasm3 - high performance WebAssembly interpreter written in C.
 * Copyright © 2020 Volodymyr Shymanskyy, Steven Massey.
 * All rights reserved.
 */

#include <wasm3.h>
#include <m3_env.h>

/*
 * Configuration
 */
#define FIB_ARG             "24"
#define WASM_STACK_SLOTS    1024

/*
 * WebAssembly app (recursive Fibonacci)
 */

unsigned char fib_wasm[] = {
  0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00, 0x01, 0x06, 0x01, 0x60,
  0x01, 0x7f, 0x01, 0x7f, 0x03, 0x02, 0x01, 0x00, 0x07, 0x07, 0x01, 0x03,
  0x66, 0x69, 0x62, 0x00, 0x00, 0x0a, 0x1f, 0x01, 0x1d, 0x00, 0x20, 0x00,
  0x41, 0x02, 0x49, 0x04, 0x40, 0x20, 0x00, 0x0f, 0x0b, 0x20, 0x00, 0x41,
  0x02, 0x6b, 0x10, 0x00, 0x20, 0x00, 0x41, 0x01, 0x6b, 0x10, 0x00, 0x6a,
  0x0f, 0x0b
};

/*
 * Engine start, liftoff!
 */

#define FATAL(func, msg) { Serial.print("Fatal: " func " "); Serial.println(msg); return; }
#define TSTART()         { tstart = micros(); }
#define TFINISH(s)       { tend = micros(); Serial.print(s " in "); Serial.print(tend-tstart); Serial.println(" us"); }

void wasm_task(void*)
{
    uint32_t tend, tstart;
    TSTART();

    M3Result result = m3Err_none;

    IM3Environment env = m3_NewEnvironment ();
    if (!env) FATAL("NewEnvironment", "failed");

    IM3Runtime runtime = m3_NewRuntime (env, WASM_STACK_SLOTS, NULL);
    if (!runtime) FATAL("NewRuntime", "failed");

    IM3Module module;
    result = m3_ParseModule (env, &module, fib_wasm, sizeof(fib_wasm)-1);
    if (result) FATAL("ParseModule", result);

    result = m3_LoadModule (runtime, module);
    if (result) FATAL("LoadModule", result);

    IM3Function f;
    result = m3_FindFunction (&f, runtime, "fib");
    if (result) FATAL("FindFunction", result);

    TFINISH("Init");

    Serial.println("Running fib(" FIB_ARG ")...");

    TSTART();

    const char* i_argv[2] = { FIB_ARG , NULL };
    result = m3_CallWithArgs (f, 1, i_argv);

    TFINISH("Done");

    if (result == m3Err_none) {
        uint32_t value = *(uint32_t*)(runtime->stack);
        Serial.print("Result: ");
        Serial.println(value);
    } else {
        M3ErrorInfo info;
        m3_GetErrorInfo (runtime, &info);
        Serial.print("Error: ");
        Serial.print(result);
        Serial.print(" (");
        Serial.print(info.message);
        Serial.println(")");
        if (info.file && strlen(info.file) && info.line) {
            Serial.print("At ");
            Serial.print(info.file);
            Serial.print(":");
            Serial.println(info.line);
        }
    }
#ifdef ESP32
    vTaskDelete(NULL);
#endif
}

void setup()
{
    Serial.begin(115200);
    delay(100);

    // Wait for serial port to connect
    // Needed for native USB port only
    while(!Serial) {}

    Serial.println("\nWasm3 v" M3_VERSION " (" M3_ARCH "), build " __DATE__ " " __TIME__);

#ifdef ESP32
    // On ESP32, we can launch in a separate thread (with 16Kb stack)
    Serial.println("Running a separate task");
    xTaskCreate(&wasm_task, "wasm3", 16*1024, NULL, 5, NULL);
#else
    wasm_task(NULL);
#endif
}

void loop()
{
    delay(100);
}
