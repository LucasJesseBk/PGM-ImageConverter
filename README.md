# PGM Image Converter & Sketch Viewer

This repository contains two projects written in C:

1. **Sketch Viewer** – A small program to view `.sk` files.
2. **PGM Image Converter** – A converter from `.pgm` files to `.sk` format.

Both projects use `clang` and support AddressSanitizer/UndefinedBehaviorSanitizer for debugging.

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
├── images/ # Place your .pgm or .sk files here
└── bin/ # Compiled binaries (sketch, test, image_converter)


---

## Building

Make sure you have `clang` installed. Then, run:

```bash
make sketch          # Builds bin/sketch
make test            # Builds bin/test
make image_converter # Builds bin/image_converter
```

## Running Tests
./bin/test

## Running Project
make run_converter FILE=images/(filename).pgm
make run FILE=images/(filename).sk
