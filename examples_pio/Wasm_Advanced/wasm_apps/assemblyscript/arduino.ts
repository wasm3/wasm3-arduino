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

@external("getGreeting")
declare function _getGreeting(ptr: usize, maxlen: usize): void;

@external("print")
declare function _print(ptr: usize, len: usize): void;

export function print(str: string): void {
  const buffer = String.UTF8.encode(str);
  _print(changetype<usize>(buffer), buffer.byteLength);
}

export function println(str: string): void {
  print(str + '\n');
}

export function getGreeting(): string {
  const arr = new ArrayBuffer(64);
  _getGreeting(changetype<usize>(arr), 64);
  return String.UTF8.decode(arr, true);
}
