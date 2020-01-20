(module
 (type $i32_i32_=>_none (func (param i32 i32)))
 (type $none_=>_none (func))
 (type $i32_=>_none (func (param i32)))
 (type $none_=>_i32 (func (result i32)))
 (import "arduino" "getPinLED" (func $app/getPinLED (result i32)))
 (import "arduino" "pinMode" (func $app/pinMode (param i32 i32)))
 (import "arduino" "digitalWrite" (func $app/digitalWrite (param i32 i32)))
 (import "arduino" "delay" (func $app/delay (param i32)))
 (memory $0 0)
 (global $app/LED (mut i32) (i32.const 0))
 (export "memory" (memory $0))
 (export "_start" (func $app/_start))
 (func $app/_start (; 4 ;)
  call $app/getPinLED
  global.set $app/LED
  global.get $app/LED
  i32.const 1
  call $app/pinMode
  loop $while-continue|0
   global.get $app/LED
   i32.const 1
   call $app/digitalWrite
   i32.const 100
   call $app/delay
   global.get $app/LED
   i32.const 0
   call $app/digitalWrite
   i32.const 900
   call $app/delay
   br $while-continue|0
  end
  unreachable
 )
)
