(module
 (type $FUNCSIG$i (func (result i32)))
 (type $FUNCSIG$v (func))
 (type $FUNCSIG$vii (func (param i32 i32)))
 (type $FUNCSIG$vi (func (param i32)))
 (import "arduino" "getPinLED" (func $arduino/getPinLED (result i32)))
 (import "arduino" "pinMode" (func $arduino/pinMode (param i32 i32)))
 (import "arduino" "digitalWrite" (func $arduino/digitalWrite (param i32 i32)))
 (import "arduino" "delay" (func $arduino/delay (param i32)))
 (memory $0 0)
 (global $app/LED (mut i32) (i32.const 0))
 (export "memory" (memory $0))
 (export "_start" (func $app/_start))
 (start $start)
 (func $app/_start (; 4 ;) (type $FUNCSIG$v)
  global.get $app/LED
  i32.const 1
  call $arduino/pinMode
  loop $continue|0
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
   br $continue|0
  end
  unreachable
 )
 (func $start (; 5 ;) (type $FUNCSIG$v)
  call $arduino/getPinLED
  global.set $app/LED
 )
 (func $null (; 6 ;) (type $FUNCSIG$v)
  nop
 )
)
