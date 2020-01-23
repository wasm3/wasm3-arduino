import * as dev from "./arduino";

let LED = dev.getPinLED();

function cbk(size: usize): usize {
  dev.println("!CBK!");
  return 123;
}

function setup(): void {
  dev.pinMode(LED, dev.OUTPUT);

  dev.print('Hello from AssemblyScript 😊\n');
}

function run(): void {
  const t = dev.millis();
  dev.println('[' + t.toString() + '] ' + dev.getString());

  //dev.testCallback(cbk);

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
  while (1) {
    run();
  }
}

