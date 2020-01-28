package main

import (
  . "./arduino"
  "strconv"
)

var LED = GetPinLED()

func setup() {
    PinMode(LED, 1)

    Println("TinyGo is running 😎")
    Println("Greeting: " + GetGreeting())
}

func loop() {
    time := Millis()
    Println(strconv.FormatUint(uint64(time), 10))

    DigitalWrite(LED, HIGH)
    Delay(100)
    DigitalWrite(LED, LOW)
    Delay(900)
}

/*
 * Entry point
 */

func main() {
    setup()
    for {
        loop()
    }
}
