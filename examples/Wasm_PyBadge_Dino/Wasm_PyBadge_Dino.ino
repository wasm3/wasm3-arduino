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
 *   
 * Note: In Arduino IDE, select Tools->Optimize->Faster (-O3)
 */
#include "dino.wasm.h"

/*
 * Engine start, liftoff!
 */

#define FATAL(func, msg) { Serial.print("Fatal: " func " "); Serial.println(msg); while(1) { delay(100); } }
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

IM3Environment  env;
IM3Runtime      runtime;
IM3Module       module;
IM3Function     func_run;
uint8_t*        mem;

void load_wasm()
{
    M3Result result = m3Err_none;
    
    if (!env) {
      env = m3_NewEnvironment ();
      if (!env) FATAL("NewEnvironment", "failed");
    }

    m3_FreeRuntime(runtime);

    runtime = m3_NewRuntime (env, 1024, NULL);
    if (!runtime) FATAL("NewRuntime", "failed");

    result = m3_ParseModule (env, &module, dino_wasm, sizeof(dino_wasm));
    if (result) FATAL("ParseModule", result);

    result = m3_LoadModule (runtime, module);
    if (result) FATAL("LoadModule", result);

    result = LinkImports (runtime);
    if (result) FATAL("LinkImports", result);

    mem = m3_GetMemory (runtime, NULL, 0);
    if (!mem) FATAL("GetMemory", "failed");
    
    result = m3_FindFunction (&func_run, runtime, "run");
    if (result) FATAL("FindFunction", result);
}

void init_random()
{
    // Try to randomize seed
    randomSeed((analogRead(A5) << 16) + analogRead(A4));
    Serial.print("Random: 0x"); Serial.println(random(INT_MAX), HEX);
}

void init_arcada()
{
    if (!arcada.arcadaBegin()) {
      FATAL("arcadaBegin", "failed");
    }
    arcada.displayBegin();
    arcada.setBacklight(128);
}

void display_info()
{
    arcada.display->fillScreen(ARCADA_WHITE);
    arcada.display->setTextColor(ARCADA_BLACK);
    arcada.display->setTextWrap(true);
    arcada.display->setCursor(0, 5);
    arcada.display->println(" Wasm3 v" M3_VERSION " (" M3_ARCH "@" + String(F_CPU/1000000) + "MHz)\n");
    arcada.display->println(" Dino game");
    arcada.display->println(" by Ben Smith (binji)");    
}

void setup()
{
    Serial.begin(115200);

    // Wait for serial port to connect
    // Needed for native USB port only
    //while(!Serial) {}

    uint32_t tend, tstart;
    TSTART();
      init_random();
      init_arcada();
      display_info();
    TFINISH("Arcada init");

    TSTART();
      load_wasm();
    TFINISH("Wasm3 init");

    Serial.println("Running WebAssembly...");

    M3Result result;
    const char* i_argv[1] = { NULL };

    while (true) {
      const uint32_t framestart = millis();

      // Process inputs
      uint32_t pressed_buttons = arcada.readButtons();
      if (arcada.justPressedButtons() & ARCADA_BUTTONMASK_START) {
        //NVIC_SystemReset();

        // Restart Dino game
        init_random();
        display_info();
        load_wasm();
      }

      uint32_t* input = (uint32_t*)(mem + 0x0000);

      *input = 0;
      if (pressed_buttons & ARCADA_BUTTONMASK_A) { // Up
        *input |= 0x1;
      }
      if (pressed_buttons & ARCADA_BUTTONMASK_B) { // Down
        *input |= 0x2;
      }

      // Render frame
      result = m3_CallWithArgs (func_run, 0, i_argv);
      if (result) break;

      // Output to display
      arcada.display->drawRGBBitmap(0, 40, (uint16_t*)(mem+0x5000), 160, 75);

      const uint32_t frametime = millis() - framestart;
      //Serial.print("FPS: "); Serial.println(1000/frametime);

      // Limit to 50..70 fps, depending on CPU/overclock setting (120..200MHz)
      //const int target_frametime = 1000/map(F_CPU/1000000, 120, 200, 50, 70);
      const uint32_t target_frametime = 1000/50;
      if (target_frametime > frametime) {
        delay(target_frametime - frametime);
      }
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
