CC = clang
CFLAGS = -std=c11 -Wall -pedantic -g
SANITIZERS = -fsanitize=address -fsanitize=undefined
LIBS = -lSDL2
INCLUDES = -I/usr/include/SDL2

# This defines the variable for use in the shell
RUN_ASAN = ASAN_OPTIONS=allocator_may_return_null=1 LSAN_OPTIONS=suppressions=asan.supp

# Default image directory
IMG_DIR = src/images

BIN = bin

sketch: src/sketch.c src/displayfull.c
	mkdir -p $(BIN)
	$(CC) $(CFLAGS) src/sketch.c src/displayfull.c $(INCLUDES) $(LIBS) -o $(BIN)/sketch $(SANITIZERS)

test: src/sketch.c src/test.c
	mkdir -p $(BIN)
	$(CC) $(CFLAGS) -DTESTING src/sketch.c src/test.c $(INCLUDES) $(LIBS) -o $(BIN)/test $(SANITIZERS)

run: bin/sketch
	$(RUN_ASAN) ./bin/sketch $(if $(findstring /,$(FILE)),$(FILE),$(IMG_DIR)/$(FILE))

BIN = bin
TARGET = image_converter


$(TARGET):
	mkdir -p $(BIN)
	clang -std=c11 -Wall -pedantic -g \
	    src/converter/main.c src/converter/pgm_header.c src/converter/sk_writer.c src/converter/tests.c \
	    -o $(BIN)/$(TARGET) \
	    -fsanitize=undefined -fsanitize=address

%: src/%.c
	clang -Dtest_$@ -std=c11 -Wall -pedantic -g src/$@.c -o $@ \
	    -fsanitize=undefined -fsanitize=address

