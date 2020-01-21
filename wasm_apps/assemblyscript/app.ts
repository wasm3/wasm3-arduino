@external("arduino", "millis")
declare function millis(): u32;

@external("arduino", "delay")
declare function delay(ms: u32): void;

@external("arduino", "pinMode")
declare function pinMode(pin: u32, mode: u32): void;

@external("arduino", "digitalWrite")
declare function digitalWrite(pin: u32, value: u32): void;

@external("arduino", "getPinLED")
declare function getPinLED(): u32;

const OFF = 0;
const ON  = 1;

const INPUT        = 0x0;
const OUTPUT       = 0x1;
const INPUT_PULLUP = 0x2;

let LED: u32 = 0;

function setup(): void {
  LED = getPinLED();
  pinMode(LED, OUTPUT);
}

function run(): void {
  digitalWrite(LED, ON);
  delay(100);
  digitalWrite(LED, OFF);
  delay(900);
}

/*
 * Entry point
 */
export function _start(): void {
  setup();
  while (1) run();
}
