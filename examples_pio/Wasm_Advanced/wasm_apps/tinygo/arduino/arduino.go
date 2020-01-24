package arduino

const (
    LOW  = 0
    HIGH = 1

    INPUT           =  0
    OUTPUT          =  1
    INPUT_PULLUP    =  2
)

//go:wasm-module arduino
//go:export millis
func Millis() uint

//go:wasm-module arduino
//go:export delay
func Delay(ms uint)

//go:wasm-module arduino
//go:export pinMode
func PinMode(pin, mode uint)

//go:wasm-module arduino
//go:export digitalWrite
func DigitalWrite(pin, value uint)

//go:wasm-module arduino
//go:export getPinLED
func GetPinLED() uint

//go:wasm-module arduino
//go:export print
func Print(s string)

//go:wasm-module arduino
//go:export getGreeting
func _getGreeting(buf *byte, maxlen uint)

func Println(s string) {
    print(s)
    print("\n")
}

func GetGreeting() string {
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
