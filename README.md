# PGM Image Converter & Sketch Viewer

An optional extension of a university assignment focused on converting binary `.pgm` image files into a custom `.sk` format, along with a visualiser for this custom format using SDL2.

## Features

1. **Sketch Viewer** – A program to display `.sk` files.
2. **PGM Image Converter** – Converts `.pgm` files into `.sk` format.

Both programs are written in C using `clang`, with AddressSanitizer and UndefinedBehaviorSanitizer enabled for debugging.

---

## 📂 Project Structure
├── Makefile
├── README.md
├── src/
│ ├── sketch.c
│ ├── displayfull.c
│ ├── test.c
│ └── converter/
│ ├── main.c
│ ├── pgm_header.c
│ ├── sk_writer.c
│ └── tests.c
├── src/images/ # Input .pgm and .sk files
└── bin/ # Compiled binaries (sketch, test, image_converter)

## Building

Make sure you have `clang` installed, then run:

```bash
make sketch          # Builds bin/sketch
make test            # Builds bin/test
make image_converter # Builds bin/image_converter
```

## Running Tests
./bin/test

## Running the Programs

# Run the Converter
make run_converter FILE=example.pgm

# Run the Sketch Viewer
make run FILE=example.sk

## Notes
- All binaries are output to the bin/ directory.
- Sanitizers are enabled for debugging memory issues.
- Any reported memory leaks originate from external libraries (e.g. SDL2/OpenGL), not from this project.
