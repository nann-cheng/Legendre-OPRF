# Legendre-OPRF
An ARM64 arithmetic library for the Legendre PRF based distributed OPRF

Run `make` which will compile arithmetic tests and benchmarks for 64,128,192,256,512 bit sized prime fields. The executables are main64, main128, main192, main256, main512.

By running `make main64` you will only compile the 64 bit prime arithmetic (and respectively for 128, 192, 256, 512)

By running `make main64 OPT_LEVEL=FAST` you will compile ARM64 assembly code which is optimised. It speeds up the multiplications, modular reduction, etc. For other prime sizes use `main128`, etc. respectively. Currently assembly is only implemented for 64 and 128 bits. (Some functions are faster without the "FAST" flag due to the compiler unwrapping some loops and speeding up some functions, which it doesn't do when compiling assembly. This only happens for cryptographically irrelevant sized primes (64) due to cache being large enough to allow the compiler to do this speedup.)


FASTER WAY TO RUN

Another way to do all tests is to give execution priviledges to `go.sh` 
`chmod +x go.sh`

This script cleans, compiles and runs the program. Make sure execution priviledges are provided for the directory. 

`./go.sh` Makes all prime sizes and runs all tests and benchmarks

`./go.sh FAST` Makes all prime sizes with ARM64 assebly, and runs all tests and benchmark 

`./go.sh X` Makes X bit prime arithmetic, runs all tests and benchmark

`./go.sh X FAST` Makes X bit prime arithmetic with assembly, runs all tests and benchmarks





---

# Copyright 2024 NOVAK KALUĐEROVIĆ

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


