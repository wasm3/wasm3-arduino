(module
 (type $none_=>_none (func))
 (type $i32_i32_=>_none (func (param i32 i32)))
 (type $i32_=>_none (func (param i32)))
 (type $none_=>_i32 (func (result i32)))
 (import "arduino" "getPinLED" (func $arduino/getPinLED (result i32)))
 (import "arduino" "pinMode" (func $arduino/pinMode (param i32 i32)))
 (import "arduino" "digitalWrite" (func $arduino/digitalWrite (param i32 i32)))
 (import "arduino" "delay" (func $arduino/delay (param i32)))
 (memory $0 0)
 (global $app/LED (mut i32) (i32.const 0))
 (export "memory" (memory $0))
 (export "_start" (func $app/_start))
 (start $~start)
 (func $app/_start (; 4 ;)
  global.get $app/LED
  i32.const 1
  call $arduino/pinMode
  loop $while-continue|0
   global.get $app/LED
   i32.const 1
   call $arduino/digitalWrite
   i32.const 100
   call $arduino/delay
   global.get $app/LED
   i32.const 0
   call $arduino/digitalWrite
   i32.const 900
   call $arduino/delay
   br $while-continue|0
  end
  unreachable
 )
 (func $~start (; 5 ;)
  call $arduino/getPinLED
  global.set $app/LED
 )
)
