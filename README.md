# What is this?

This is Reversi game written in C++/JavaScript/HTML, which demonstrates the use of WebAssembly.

The C++ code is compiled to WebAssembly and then loaded into the browser.

The rules can be found [here](https://en.wikipedia.org/wiki/Reversi).

# How to build

## Prerequisites

* [Emscripten](https://emscripten.org/docs/getting_started/downloads.html)

## Build

```bash
make all
```

## Serve
    
```bash
make serve
```

## Clean

```bash
make clean
```

## Run tests

```bash
make tests
```