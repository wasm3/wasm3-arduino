#![no_std]

mod arduino_api;
use arduino_api::*;

struct App {
  led: u32,
}

impl App {
    fn new() -> Self {
        let led = get_pin_led();
        pin_mode(led, OUTPUT);
        App { led }
    }

    fn update(&self) {
        digital_write(self.led, HIGH);
        delay(100);
        digital_write(self.led, LOW);
        delay(900);
    }
}

#[no_mangle]
pub extern fn _start() {
    let app = App::new();
    loop { app.update() };
}

#[panic_handler]
fn handle_panic(_: &core::panic::PanicInfo) -> ! {
    loop {}
}
