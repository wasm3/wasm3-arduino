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

@external("print")        declare function _print(ptr: usize): void;
@external("getString")    declare function _getString(ptr: usize, len: i32): void;

export function print(str: string): void {
  _print(changetype<usize>(String.UTF8.encode(str, true)))
}

export function println(str: string): void {
  print(str + '\n')
}

export function getString(): string {
  const arr = new ArrayBuffer(64)
  _getString(changetype<usize>(arr), 64)
  return String.UTF8.decode(arr, true)
}

/*
type testFunc = ((size: usize) => usize) | null;

export declare function testCallback(f: testFunc): void;
*/
