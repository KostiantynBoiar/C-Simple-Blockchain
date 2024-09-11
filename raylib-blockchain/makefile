# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Wno-deprecated-declarations -O2

# Libraries to link
LIBS = -lssl -lcrypto -lpthread -lGL -lm -ldl

# Source files
SRC = main.c User.c

# Output executable
TARGET = openssl_sha256_program

# Raylib paths
RAYLIB_DIR = raylib
RAYLIB_BUILD_DIR = $(RAYLIB_DIR)/build
RAYLIB_LIB = $(RAYLIB_BUILD_DIR)/raylib/libraylib.a
RAYLIB_INCLUDE = $(RAYLIB_DIR)/src

# Make sure Raylib is built
all: $(TARGET)

$(TARGET): $(SRC) $(RAYLIB_LIB)
	$(CC) $(CFLAGS) -I$(RAYLIB_INCLUDE) -o $(TARGET) $(SRC) $(RAYLIB_LIB) $(LIBS)

$(RAYLIB_LIB):
	cd $(RAYLIB_DIR) && mkdir -p build && cd build && cmake .. && make

# Clean rule
clean:
	rm -f $(TARGET)
	rm -f $(RAYLIB_LIB)
	rm -rf $(RAYLIB_DIR)/build

# Phony target to avoid conflicts with files named 'clean'
.PHONY: clean all
