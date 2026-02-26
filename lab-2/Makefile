CC := g++
CFLAGS := -Wall -Wextra -Isrc -lm -Wno-missing-field-initializers `pkg-config --libs opencv4` `pkg-config --cflags opencv4` -Wno-unused-function -g -O0

SRC_DIR := src
BIN_DIR := bin

SRC := $(wildcard $(SRC_DIR)/*.cpp);
BIN := $(BIN_DIR)/main

.PHONY: all debug clean

all: $(BIN_DIR) $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BIN_DIR)

$(BIN_DIR):
	mkdir -p $@
