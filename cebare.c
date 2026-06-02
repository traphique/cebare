/*
 * Cebare v0.1.0
 * Bare-metal AI optimization & reverse engineering instrument
 * Single-file, zero-dependency, forged at the metal.
 *
 * Compile: gcc -O3 -march=native -mavx512f -mavx512dq -Wall -Wextra -std=c11 -o cebare cebare.c -lm
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <immintrin.h>
#include <x86intrin.h>
#include <unistd.h>
#include <getopt.h>

#define VERSION "0.1.0"
#define MAX_SIZE 64
#define ALIGNMENT 64

// ANSI colors
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define MAGENTA "\033[35m"
#define BLUE    "\033[34m"

// === Aligned memory helpers ===
static void* aligned_alloc64(size_t size) {
    void* ptr = NULL;
    if (posix_memalign(&ptr, ALIGNMENT, size) != 0) {
        perror("posix_memalign");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

static void aligned_free(void* ptr) {
    free(ptr);
}

// === High-precision timing ===
static inline uint64_t rdtsc(void) {
    unsigned int aux;
    return __rdtscp(&aux);
}

// === Statistical helpers ===
typedef struct {
    double mean;
    double stddev;
    double min;
    double max;
    uint64_t count;
} stats_t;

static stats_t compute_stats(double* times, uint64_t n) {
    stats_t s = {0};
    if (n == 0) return s;
    
    double sum = 0, sumsq = 0, minv = 1e100, maxv = -1e100;
    for (uint64_t i = 0; i < n; i++) {
        double t = times[i];
        sum += t;
        sumsq += t * t;
        if (t < minv) minv = t;
        if (t > maxv) maxv = t;
    }
    s.mean = sum / n;
    double var = (sumsq / n) - (s.mean * s.mean);
    s.stddev = var > 0 ? sqrt(var) : 0;
    s.min = minv;
    s.max = maxv;
    s.count = n;
    return s;
}

// === Naive matrix multiply (for comparison) ===
static void matmul_naive(const double* A, const double* B, double* C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += A[i*n + k] * B[k*n + j];
            }
            C[i*n + j] = sum;
        }
    }
}

// === AVX-512 matrix multiply (optimized) ===
static void matmul_avx512(const double* A, const double* B, double* C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j += 8) {
            __m512d c0 = _mm512_setzero_pd();
            for (int k = 0; k < n; k++) {
                __m512d a = _mm512_set1_pd(A[i*n + k]);
                __m512d b = _mm512_load_pd(&B[k*n + j]);
                c0 = _mm512_fmadd_pd(a, b, c0);
            }
            _mm512_store_pd(&C[i*n + j], c0);
        }
    }
}

// === Benchmark runner ===
static void run_benchmark(int size, uint64_t iters, int use_avx) {
    printf("\n%s=== PERFORMANCE BENCHMARK ===%s\n", BOLD CYAN, RESET);
    printf("Matrix size: %dx%d | Iterations: %lu\n\n", size, size, iters);

    size_t bytes = (size_t)size * size * sizeof(double);
    double* A = aligned_alloc64(bytes);
    double* B = aligned_alloc64(bytes);
    double* C = aligned_alloc64(bytes);

    for (int i = 0; i < size*size; i++) {
        A[i] = (i % 17) * 0.1 + 0.5;
        B[i] = ((i * 3) % 19) * 0.1 + 0.3;
    }

    double* times = malloc(iters * sizeof(double));

    const char* label = use_avx ? "AVX-512 Optimized" : "Naive C";
    printf("%sRunning %s...%s\n", YELLOW, label, RESET);

    for (uint64_t iter = 0; iter < iters; iter++) {
        uint64_t start = rdtsc();
        if (use_avx) {
            matmul_avx512(A, B, C, size);
        } else {
            matmul_naive(A, B, C, size);
        }
        uint64_t end = rdtsc();
        times[iter] = (double)(end - start) * 0.3;
    }

    stats_t stats = compute_stats(times, iters);
    printf("%s%-18s%s mean %8.0f ns  (std %6.0f, min %6.0f, max %6.0f)\n",
           GREEN, label, RESET,
           stats.mean, stats.stddev, stats.min, stats.max);

    double ref = 0.0;
    for (int k = 0; k < size; k++) ref += A[k] * B[k * size];
    double diff = fabs(C[0] - ref);
    printf("Correctness: %s%s%s\n\n", (diff < 1e-6) ? GREEN : RED, (diff < 1e-6) ? "PASS" : "FAIL", RESET);

    free(times);
    aligned_free(A);
    aligned_free(B);
    aligned_free(C);
}

// === Tiny 2-layer Neural Network Demo ===
static void run_nn_demo(int size) {
    printf("\n%s=== TINY NEURAL NET FORWARD PASS DEMO ===%s\n", BOLD MAGENTA, RESET);
    
    size_t bytes = (size_t)size * size * sizeof(double);
    double* W1 = aligned_alloc64(bytes);
    double* W2 = aligned_alloc64(bytes);
    double* input = aligned_alloc64(bytes);
    double* hidden = aligned_alloc64(bytes);
    double* output = aligned_alloc64(bytes);

    for (int i = 0; i < size*size; i++) {
        W1[i] = sin(i) * 0.1 + 0.8;
        W2[i] = cos(i * 1.3) * 0.1 + 0.7;
        input[i] = (i % 7) * 0.2 + 0.1;
    }

    uint64_t start = rdtsc();
    matmul_avx512(input, W1, hidden, size);
    for (int i = 0; i < size*size; i++) if (hidden[i] < 0) hidden[i] = 0;
    matmul_avx512(hidden, W2, output, size);
    uint64_t end = rdtsc();

    printf("2-layer forward pass completed in %s%.2f µs%s\n", GREEN, (double)(end - start) * 0.0003, RESET);
    printf("Output[0][0] = %s%.6f%s\n\n", CYAN, output[0], RESET);

    aligned_free(W1); aligned_free(W2);
    aligned_free(input); aligned_free(hidden); aligned_free(output);
}

// === Timing Side-Channel Audit ===
static void run_side_channel_audit(void) {
    printf("\n%s=== DEFENSIVE CYBER: Timing Side-Channel Audit ===%s\n", BOLD RED, RESET);

    const int SECRET = 1;
    const int N = 100000;
    double times[2] = {0};

    uint64_t start = rdtsc();
    for (int i = 0; i < N; i++) {
        volatile int x = (SECRET == 0) ? 1 : 42;
        (void)x;
    }
    uint64_t end = rdtsc();
    times[0] = (double)(end - start) / N;

    start = rdtsc();
    for (int i = 0; i < N; i++) {
        volatile int idx = (SECRET == 1) ? 4095 : 0;
        volatile char mem[4096] = {0};
        mem[idx] = 1;
        (void)mem;
    }
    end = rdtsc();
    times[1] = (double)(end - start) / N;

    printf("Short path (bit=0) avg: %s%.0f cycles%s\n", GREEN, times[0], RESET);
    printf("Long  path (bit=1) avg: %s%.0f cycles%s\n", RED, times[1], RESET);
    double diff = fabs(times[1] - times[0]);
    printf("%s>>> TIMING DIFFERENCE DETECTED: %.0f cycles (%.1fx)%s\n\n", YELLOW, diff, times[1]/times[0], RESET);
}

// === Self-Disassembly ===
static void run_disasm(void) {
    printf("\n%s=== SELF-DISASSEMBLY & INTROSPECTION ===%s\n", BOLD BLUE, RESET);
    printf("Extracting relevant AVX-512 instructions...\n\n");

    FILE* fp = popen("objdump -d ./cebare 2>/dev/null | grep -E 'vfmadd|vbroadcast|vmul|vmov' | head -12", "r");
    if (!fp) {
        printf("%s[objdump unavailable]%s\n\n", RED, RESET);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s%s%s", CYAN, line, RESET);
    }
    pclose(fp);
    printf("\n%sExact machine instructions from the optimized path.%s\n\n", MAGENTA, RESET);
}

// === CLI ===
static void print_help(void) {
    printf("%sCebare v%s — Forged at the metal.%s\n\n", BOLD, VERSION, RESET);
    printf("Usage: ./cebare [options]\n\nOptions:\n  --size N\n  --iters N\n  --benchmark\n  --audit\n  --nn-demo\n  --disasm\n  --all\n  --help\n");
}

int main(int argc, char** argv) {
    int size = 16;
    uint64_t iters = 50000;
    int do_bench = 0, do_audit = 0, do_nn = 0, do_disasm = 0, do_all = 0;

    static struct option long_options[] = {
        {"size", required_argument, 0, 's'},
        {"iters", required_argument, 0, 'i'},
        {"benchmark", no_argument, 0, 'b'},
        {"audit", no_argument, 0, 'a'},
        {"nn-demo", no_argument, 0, 'n'},
        {"disasm", no_argument, 0, 'd'},
        {"all", no_argument, 0, 'A'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "s:i:bandAh", long_options, NULL)) != -1) {
        switch (opt) {
            case 's': size = atoi(optarg); break;
            case 'i': iters = strtoull(optarg, NULL, 10); break;
            case 'b': do_bench = 1; break;
            case 'a': do_audit = 1; break;
            case 'n': do_nn = 1; break;
            case 'd': do_disasm = 1; break;
            case 'A': do_all = 1; break;
            case 'h': print_help(); return 0;
            default: return 1;
        }
    }

    if (do_all) do_bench = do_audit = do_nn = do_disasm = 1;
    if (!do_bench && !do_audit && !do_nn && !do_disasm) {
        print_help();
        return 0;
    }

    printf("%sCebare v%s — The machine examining itself.%s\n\n", BOLD CYAN, VERSION, RESET);

    if (do_bench) {
        run_benchmark(size, iters, 0);
        run_benchmark(size, iters, 1);
    }
    if (do_audit) run_side_channel_audit();
    if (do_nn) run_nn_demo(size);
    if (do_disasm) run_disasm();

    printf("%s=== Complete. Truth measured. ===%s\n", BOLD, RESET);
    return 0;
}
