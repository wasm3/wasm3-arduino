# Compile
rustc -O --crate-type=cdylib              \
      --target=wasm32-unknown-unknown     \
      -C link-arg=-zstack-size=2048       \
      -C link-arg=-s                      \
      -o app.wasm app.rs

# Optimize (optional)
wasm-opt -O3 app.wasm -o app.wasm
wasm-strip app.wasm

# Convert to WAT
#wasm2wat app.wasm -o app.wat

# Convert to C header
xxd -i app.wasm > app.wasm.h
