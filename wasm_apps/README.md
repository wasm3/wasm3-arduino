## WebAssembly example apps

## C++ app

`./wasm_cpp` directory contains an example Arduino app (sketch) that is compiled to WebAssembly.  
Compilation is performed using `wasicc` here, but `clang --target=wasm32` can be used as well.  
See `build.sh` for details.

## Rust app

For Rust, the LED pin is currently hardcoded in the app.  
Before building the app, please install the toolchain:
```sh
rustup target add wasm32-unknown-unknown
```
To rebuild, use `build.sh`.

## AssemblyScript app

For AssemblyScript the LED pin is currently hardcoded in the app.  
Before building the app, please install npm dependencies:
```sh
yarn install
# or: npm install
```
To rebuild, use `npm run build` or `build.sh`.

## TinyGo app

For TinyGo, the LED pin is currently hardcoded in the app.  
To rebuild, use `build.sh`.


**Note:** for all examples, the resulting `wasm` binary is converted to a C header using `xxd`.
