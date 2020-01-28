import * as dev from "./arduino";

let LED = dev.getPinLED();

function setup(): void {
  dev.pinMode(LED, dev.OUTPUT);

  dev.println('AssemblyScript is running 😎')
  dev.print('Greeting: ' + dev.getGreeting() + '\n');
}

function run(): void {
  const time = dev.millis();
  dev.println(time.toString());

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
