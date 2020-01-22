package main

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


/*
 * App
 */

const LED uint = 13
//var LED = getPinLED()

func setup() {
    pinMode(LED, 1)
}

func loop() {
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
