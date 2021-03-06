/*
 * Wasm3 - high performance WebAssembly interpreter written in C.
 * Copyright © 2020 Volodymyr Shymanskyy, Steven Massey.
 * All rights reserved.
 */

#include <wasm3.h>
#include <m3_api_defs.h>
#include <m3_env.h>

#include "coremark_minimal.wasm.h"

m3ApiRawFunction(env_clock_ms)
{
    m3ApiReturnType (uint32_t)
    m3ApiReturn(millis());
}

IM3Environment  env;
IM3Runtime      runtime;
IM3Module       module;
IM3Function     func_run;

#define FATAL(func, msg) { Serial.print("Fatal: " func " "); Serial.println(msg); while(1) { delay(100); } }

void wasm_task(void*)
{
    M3Result result = m3Err_none;

    result = m3_FindFunction (&func_run, runtime, "run");
    if (result) FATAL("FindFunction", result);

    Serial.println("Running CoreMark 1.0...");

    result = m3_CallV (func_run);
    if (result) FATAL("Call", result);

    float value = 0;
    result = m3_GetResultsV (func_run, &value);
    if (result) FATAL("GetResults: %s", result);

    Serial.print("Result: ");
    Serial.println(value, 3);

    if (result != m3Err_none) {
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
    delay(500);

    // Wait for serial port to connect
    // Needed for native USB port only
    //while(!Serial) {}

    Serial.println();
    Serial.println("Wasm3 v" M3_VERSION " build " __DATE__ " " __TIME__);
    Serial.println("CPU: " M3_ARCH " @ " + String(F_CPU/1000000) + " MHz");

    M3Result result = m3Err_none;

    env = m3_NewEnvironment ();
    if (!env) FATAL("NewEnvironment", "failed");

    runtime = m3_NewRuntime (env, 2048, NULL);
    if (!runtime) FATAL("NewRuntime", "failed");

    runtime->memoryLimit = 3*1024;

    result = m3_ParseModule (env, &module, coremark_minimal_wasm, coremark_minimal_wasm_len);
    if (result) FATAL("ParseModule", result);

    result = m3_LoadModule (runtime, module);
    if (result) FATAL("LoadModule", result);

    m3_LinkRawFunction (module, "env",    "clock_ms",   "i()",      &env_clock_ms);

#if defined(ESP8266)
    ESP.wdtDisable();
    *((volatile uint32_t*) 0x60000900) &= ~(1); // Hardware WDT OFF
    wasm_task(NULL);
#elif defined(ESP32)
    Serial.println("Launching a separate task");
    disableCore0WDT();
    xTaskCreate(&wasm_task, "wasm3", 64*1024, NULL, 5, NULL);
#else
    wasm_task(NULL);
#endif
}

void loop()
{
    delay(100);
}
