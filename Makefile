# Makefile for Cebare

CC = gcc
CFLAGS = -O3 -march=native -mavx512f -mavx512dq -Wall -Wextra -std=gnu11
LIBS = -lm

TARGET = cebare
SRC = cebare.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)
	@echo "Build complete: ./$(TARGET) --help"

run: $(TARGET)
	./$(TARGET) --size 8 --iters 50000 --benchmark --audit --nn-demo --disasm

disasm: $(TARGET)
	objdump -d -M intel $(TARGET) | grep -A 150 '<optimized_gemm>' | head -100

clean:
	rm -f $(TARGET) *.o

.PHONY: all run disasm clean