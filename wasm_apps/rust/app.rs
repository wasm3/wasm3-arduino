#![no_std]

mod arduino_api;
use arduino_api::*;

static mut LED: u32 = 0;

unsafe fn setup() {
    LED = get_pin_led();
    pin_mode(LED, OUTPUT);
}

unsafe fn run() {
    digital_write(LED, HIGH);
    delay(100);
    digital_write(LED, LOW);
    delay(900);
}

/*
 * Entry point
 */
#[no_mangle]
pub extern fn _start() {
    unsafe {
        setup();
        loop {
             run();
        }
    }
}

#[panic_handler]
fn handle_panic(_: &core::panic::PanicInfo) -> ! {
    loop {}
}
