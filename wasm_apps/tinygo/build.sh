# Prepare
export PATH=/opt/tinygo0.17.0/bin:$PATH
export PATH=/opt/go1.15.2/bin:$PATH
export GOROOT=/opt/go1.15.2

# Compile
tinygo  build -target wasm                \
        -panic trap -wasm-abi generic     \
        -ldflags="-z stack-size=2048 --max-memory=65536" \
        -o app.wasm app.go

# Optimize (optional)
wasm-opt -Os app.wasm -o app.wasm
wasm-strip app.wasm

# Convert to WAT
#wasm2wat --generate-names app.wasm -o app.wat

# Convert to C header
xxd -i app.wasm > app.wasm.h
