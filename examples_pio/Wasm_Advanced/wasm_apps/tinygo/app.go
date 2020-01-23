package main

import (
  "strconv"
)

/*
 * Arduino API
 */

const (
    LOW  = 0
    HIGH = 1

    INPUT           =  0
    OUTPUT          =  1
    INPUT_PULLUP    =  2
)

//go:wasm-module arduino
//go:export millis
func millis() uint

//go:wasm-module arduino
//go:export delay
func delay(ms uint)

//go:wasm-module arduino
//go:export pinMode
func pinMode(pin, mode uint)

//go:wasm-module arduino
//go:export digitalWrite
func digitalWrite(pin, value uint)

//go:wasm-module arduino
//go:export getPinLED
func getPinLED() uint

//go:wasm-module arduino
//go:export print
func print(s string)

//go:wasm-module arduino
//go:export getGreeting
func _getGreeting(buf *byte, maxlen uint)

func println(s string) {
    print(s + "\n")
}

func getGreeting() string {
    var buf = make([]byte, 64)
    _getGreeting(&buf[0], 64)
    // Find '\0'
    n := -1
    for i, b := range buf {
        if b == 0 {
            break
        }
        n = i
    }
    return string(buf[:n+1])
}

/*
 * App
 */

var LED = getPinLED()

func setup() {
    pinMode(LED, 1)

    println("TinyGo is running")
    println("Greeting: " + getGreeting())
}

func loop() {
    t := millis()
    println(strconv.FormatUint(uint64(t), 10))

    digitalWrite(LED, HIGH)
    delay(100)
    digitalWrite(LED, LOW)
    delay(900)
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
