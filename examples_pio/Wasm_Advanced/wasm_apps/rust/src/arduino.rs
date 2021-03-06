#![allow(dead_code)]
#![allow(non_snake_case)]

#[link(wasm_import_module = "arduino")]
extern {
  #[link_name = "millis"]         fn _millis() -> u32;
  #[link_name = "delay"]          fn _delay(ms: u32);
  #[link_name = "pinMode"]        fn _pinMode(pin: u32, mode: u32);
  #[link_name = "digitalWrite"]   fn _digitalWrite(pin: u32, value: u32);
  #[link_name = "getPinLED"]      fn _getPinLED() -> u32;
  #[link_name = "getGreeting"]    fn _getGreeting(utf8: * mut u8, maxlen: usize);
  #[link_name = "print"]          fn _print(utf8: *const u8, len: usize);
}

pub static LOW: u32  = 0;
pub static HIGH: u32 = 1;

pub static INPUT: u32        = 0x0;
pub static OUTPUT: u32       = 0x1;
pub static INPUT_PULLUP: u32 = 0x2;

pub fn millis         () -> u32               { unsafe { _millis() } }
pub fn delay          (ms: u32)               { unsafe { _delay(ms); } }
pub fn pinMode        (pin: u32, mode: u32)   { unsafe { _pinMode(pin, mode) } }
pub fn digitalWrite   (pin: u32, value: u32)  { unsafe { _digitalWrite(pin, value) } }
pub fn getPinLED      () -> u32               { unsafe { _getPinLED() } }

pub fn print(string: &str) {
    unsafe { _print(string.as_ptr(), string.len()) }
}

pub fn println(string: &str) {
    print(string);
    print("\n");
}

//TODO: getGreeting

#[panic_handler]
fn handle_panic(_: &core::panic::PanicInfo) -> ! {
    unsafe { core::arch::wasm32::unreachable() }
}
