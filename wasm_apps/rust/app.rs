#![feature(core_intrinsics)]
#![no_std]

mod arduino;
use arduino::*;

struct App {
  led: u32,
}

impl App {
    fn new() -> Self {
        let led = get_pin_led();
        pin_mode(led, OUTPUT);
        Self { led }
    }

    fn update(&self) {
        digital_write(self.led, ON);
        delay(100);
        digital_write(self.led, OFF);
        delay(900);
    }
}

#[no_mangle]
pub extern fn _start() {
    let app = App::new();
    loop { app.update() };
}