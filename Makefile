# Makefile for Cebare - modular multi-translation-unit build.

CC      := gcc
CFLAGS  := -O3 -march=native -mavx512f -mavx512dq -Wall -Wextra -std=gnu11 \
           -Iinclude -MMD -MP
LDLIBS  := -lm

TARGET  := cebare
BUILD   := build

# Every module except main.c forms the reusable "library" the binary and the
# tests both link against.
LIB_SRCS  := $(filter-out src/main.c,$(wildcard src/*.c))
LIB_OBJS  := $(LIB_SRCS:src/%.c=$(BUILD)/%.o)
MAIN_OBJ  := $(BUILD)/main.o

TEST_SRCS := $(wildcard tests/*.c)
TEST_BINS := $(TEST_SRCS:tests/%.c=$(BUILD)/%)

DEPS := $(LIB_OBJS:.o=.d) $(MAIN_OBJ:.o=.d)

all: $(TARGET)

$(TARGET): $(MAIN_OBJ) $(LIB_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
	@echo "Build complete: ./$(TARGET) --help"

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

# Each test binary links its own object plus the full module library.
$(BUILD)/%: tests/%.c $(LIB_OBJS) | $(BUILD)
	$(CC) $(CFLAGS) -o $@ $< $(LIB_OBJS) $(LDLIBS)

$(BUILD):
	mkdir -p $(BUILD)

test: $(TEST_BINS)
	@echo "=== Running test suite ==="
	@fail=0; for t in $(TEST_BINS); do \
		echo "--- $$t ---"; \
		./$$t || fail=1; \
	done; \
	if [ $$fail -eq 0 ]; then \
		echo "=== ALL TESTS PASSED ==="; \
	else \
		echo "=== TESTS FAILED ==="; exit 1; \
	fi

run: $(TARGET)
	./$(TARGET) --size 8 --iters 50000 --benchmark --audit --nn-demo --disasm

disasm: $(TARGET)
	objdump -d -M intel $(TARGET) | grep -A 150 '<optimized_gemm>' | head -100

clean:
	rm -rf $(BUILD) $(TARGET)

-include $(DEPS)

.PHONY: all test run disasm clean
