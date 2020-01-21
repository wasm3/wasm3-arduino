package main

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

const (
	// Off turn off Led light
	Off = 0
	// On turn on Led light
	On = 1

	// Input ...
	Input = 0
	// Output ...
	Output = 1
	// InputPullUp ...
	InputPullUp = 2

	// Led (hardcoded)
	Led uint = 13
)

func setup() {
	pinMode(Led, 1)
}

func loop() {
	digitalWrite(Led, On)
	delay(100)
	digitalWrite(Led, Off)
	delay(900)
}

func main() {
	setup()
	for {
		loop()
	}
}
