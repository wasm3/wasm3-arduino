import * as dev from "./arduino";

let LED: u32 = dev.getPinLED();

function setup(): void {
  dev.pinMode(LED, dev.OUTPUT);

  dev.print('Hello from AssemblyScript 😊\n');
}

function run(): void {
  const t: u32 = dev.millis();
  dev.println('[' + t.toString() + '] ' + 'Тест юнікоду.');

  dev.digitalWrite(LED, dev.HIGH);
  dev.delay(100);
  dev.digitalWrite(LED, dev.LOW);
  dev.delay(900);
}

/*
 * Entry point
 */
export function _start(): void {
  setup();
  while (1) run();
}

