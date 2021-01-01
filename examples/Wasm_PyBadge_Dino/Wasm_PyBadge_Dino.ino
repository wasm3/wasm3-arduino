/*
 * Wasm3 - high performance WebAssembly interpreter written in C.
 * Copyright © 2020 Volodymyr Shymanskyy, Steven Massey.
 * All rights reserved.
 */

#include "Adafruit_Arcada.h"

#include <wasm3.h>
#include <m3_env.h>
#include <m3_api_defs.h>

/*
 * Dino game by by Ben Smith (binji)
 *   https://github.com/binji/raw-wasm/tree/master/dino
 * To build:
 *   export PATH=/opt/wasp/build/src/tools:$PATH
 *   wasp wat2wasm --enable-numeric-values -o dino.wasm dino.wat
 *   xxd -iC dino.wasm > dino.wasm.h
 */
#include "dino.wasm.h"

/*
 * Engine start, liftoff!
 */

#define FATAL(func, msg) { Serial.print("Fatal: " func " "); Serial.println(msg); return; }
#define TSTART()         { tstart = micros(); }
#define TFINISH(s)       { tend = micros(); Serial.print(s " in "); Serial.print(tend-tstart); Serial.println(" us"); }

// The Math.random() function returns a floating-point,
// pseudo-random number in the range 0 to less than 1
m3ApiRawFunction(Math_random)
{
    m3ApiReturnType (float)
    
    float r = (float)random(INT_MAX)/INT_MAX;
    //Serial.print("Random: "); Serial.println(r);

    m3ApiReturn(r);
}

// Memcpy is generic, and much faster in native code
m3ApiRawFunction(Dino_memcpy)
{
    m3ApiGetArgMem  (uint8_t *, dst)
    m3ApiGetArgMem  (uint8_t *, src)
    m3ApiGetArgMem  (uint8_t *, dstend)

    unsigned len = dstend-dst;
    memcpy(dst, src, len ? len : 1);

    m3ApiSuccess();
}

M3Result  LinkImports  (IM3Runtime runtime)
{
    IM3Module module = runtime->modules;

    m3_LinkRawFunction (module, "Math",   "random",     "f()",      &Math_random);
    m3_LinkRawFunction (module, "Dino",   "memcpy",     "v(iii)",   &Dino_memcpy);

    return m3Err_none;
}

Adafruit_Arcada arcada;

void setup()
{
    Serial.begin(115200);

    // Wait for serial port to connect
    // Needed for native USB port only
    //while(!Serial) {}

    // Start TFT and fill black
    if (!arcada.arcadaBegin()) {
      Serial.print("Failed to begin");
      while (1);
    }
    arcada.displayBegin();
    arcada.setBacklight(128);
  
    arcada.display->setCursor(0, 0);
    arcada.display->fillScreen(ARCADA_BLACK);
    arcada.display->setTextColor(ARCADA_WHITE);
    arcada.display->setTextSize(1);
    arcada.display->setTextWrap(true);
    arcada.display->println("Wasm3 v" M3_VERSION " (" M3_ARCH ")");
    arcada.display->println("build " __DATE__ " " __TIME__);
    arcada.display->println("\nDino game");
    arcada.display->println("by Ben Smith (binji)");
    
    // Try to randomize seed
    randomSeed((analogRead(A5) << 16) + analogRead(A4));
    Serial.print("Random: "); Serial.println(random(INT_MAX), HEX);

    uint32_t tend, tstart;
    TSTART();

    M3Result result = m3Err_none;

    IM3Environment env = m3_NewEnvironment ();
    if (!env) FATAL("NewEnvironment", "failed");

    IM3Runtime runtime = m3_NewRuntime (env, 1024, NULL);
    if (!runtime) FATAL("NewRuntime", "failed");

    IM3Module module;
    result = m3_ParseModule (env, &module, dino_wasm, sizeof(dino_wasm));
    if (result) FATAL("ParseModule", result);

    result = m3_LoadModule (runtime, module);
    if (result) FATAL("LoadModule", result);

    result = LinkImports (runtime);
    if (result) FATAL("LinkImports", result);

    IM3Function f;
    result = m3_FindFunction (&f, runtime, "run");
    if (result) FATAL("FindFunction", result);

    TFINISH("Init");

    Serial.println("Running WebAssembly...");

    const char* i_argv[1] = { NULL };
    uint8_t* mem = m3_GetMemory(runtime, NULL, 0);

    while (true) {
      const uint32_t framestart = millis();

      // Process inputs
      uint32_t pressed_buttons = arcada.readButtons();
      if (pressed_buttons & ARCADA_BUTTONMASK_START) {
        NVIC_SystemReset();
      } else if (pressed_buttons & ARCADA_BUTTONMASK_A && pressed_buttons & ARCADA_BUTTONMASK_B) {
        *(uint32_t*)(mem) = 3;
      } else if (pressed_buttons & ARCADA_BUTTONMASK_B) { // Down
        *(uint32_t*)(mem) = 2;
      } else if (pressed_buttons & ARCADA_BUTTONMASK_A) { // Up
        *(uint32_t*)(mem) = 1;
      } else {
        *(uint32_t*)(mem) = 0;
      }

      // Render frame
      result = m3_CallWithArgs (f, 0, i_argv);
      if (result) break;
      arcada.display->drawRGBBitmap(0, 50, (uint16_t*)(mem+0x5000), 160, 75);

      //Serial.print("FPS: "); Serial.println(1000/(millis() - framestart));

      // Limit to 40 fps
      while (millis() - framestart < (1000/40)) { delay(1); }
    }

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
}

void loop()
{
    delay(100);
}
