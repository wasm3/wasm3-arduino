export const LOW  = 0;
export const HIGH = 1;

export const INPUT        = 0x0;
export const OUTPUT       = 0x1;
export const INPUT_PULLUP = 0x2;

export declare function millis(): u32;
export declare function delay(ms: u32): void;
export declare function pinMode(pin: u32, mode: u32): void;
export declare function digitalWrite(pin: u32, value: u32): void;
export declare function getPinLED(): u32;

