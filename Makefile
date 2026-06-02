# Makefile for Cebare v0.1.0 - single file
CC = gcc
CFLAGS = -O3 -march=native -mavx512f -mavx512dq -Wall -Wextra -std=c11 -D_GNU_SOURCE
LDFLAGS = -lm
TARGET = cebare
SRC = cebare.c
all: $(TARGET)
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)
clean:
	rm -f $(TARGET)
run: $(TARGET)
	./$(TARGET) --all
.PHONY: all clean run