# SDL BoilerPlate

[![Build](https://github.com/paul-doughty/BoilerPlate/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/paul-doughty/BoilerPlate/actions/workflows/build.yml)

A minimal SDL2 starter project with a simple hello world window example.

## Requirements

- g++
- make
- libsdl2-dev

## Build

```sh
make
```

## Run

```sh
make run
```

## CMake

```sh
cmake -S . -B build
cmake --build build
./build/hello_sdl
```
