CC = clang
CFLAGS = -std=c11 -Wall -pedantic -g
SANITIZERS = -fsanitize=address -fsanitize=undefined
LIBS = -lSDL2
INCLUDES = -I/usr/include/SDL2

# This defines the variable for use in the shell
RUN_ASAN = ASAN_OPTIONS=allocator_may_return_null=1 LSAN_OPTIONS=suppressions=asan.supp

sketch: sketch.c displayfull.c
	$(CC) $(CFLAGS) sketch.c displayfull.c $(INCLUDES) $(LIBS) -o $@ $(SANITIZERS)

test: sketch.c test.c
	$(CC) $(CFLAGS) -DTESTING sketch.c test.c $(INCLUDES) $(LIBS) -o $@ $(SANITIZERS)

# Add a 'run' target to make it easy to execute with the flags
run: sketch
	$(RUN_ASAN) ./sketch $(FILE)


BIN = bin
TARGET = image_converter

$(TARGET):
	mkdir -p $(BIN)
	clang -std=c11 -Wall -pedantic -g \
	    converter/main.c converter/pgm_header.c converter/sk_writer.c converter/tests.c \
	    -o $(BIN)/$(TARGET) \
	    -fsanitize=undefined -fsanitize=address

%: %.c
	clang -Dtest_$@ -std=c11 -Wall -pedantic -g $@.c -o $@ \
	    -fsanitize=undefined -fsanitize=address
