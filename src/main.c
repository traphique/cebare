#define _GNU_SOURCE

#include "cebare/colors.h"
#include "cebare/bench.h"
#include "cebare/audit.h"
#include "cebare/nn.h"
#include "cebare/disasm.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

static void print_help(const char *prog) {
    printf(C_BOLD "Cebare %s" C_RESET
           " - Bare-metal AI Optimizer & Auditor\n\n", CEBARE_VERSION);
    printf("Usage: %s [options]\n\n", prog);
    printf("Options:\n");
    printf("  --size N       Matrix dimension N (NxN), default 8\n");
    printf("  --iters N      Benchmark iterations, default 50000\n");
    printf("  --benchmark    Run the naive vs AVX-512 GEMM benchmark\n");
    printf("  --audit        Run the timing side-channel audit\n");
    printf("  --nn-demo      Run the 2-layer neural-net forward pass demo\n");
    printf("  --disasm       Self-disassemble the optimized_gemm kernel\n");
    printf("  --help         Show this help and exit\n\n");
    printf("Example:\n");
    printf("  %s --size 8 --iters 50000 --benchmark --audit --nn-demo --disasm\n",
           prog);
}

int main(int argc, char **argv) {
    int size = 8;
    long iters = 50000;
    int do_bench = 0, do_audit = 0, do_nn = 0, do_disasm = 0;

    static struct option long_opts[] = {
        {"size",      required_argument, 0, 's'},
        {"iters",     required_argument, 0, 'i'},
        {"benchmark", no_argument,       0, 'b'},
        {"audit",     no_argument,       0, 'a'},
        {"nn-demo",   no_argument,       0, 'n'},
        {"disasm",    no_argument,       0, 'd'},
        {"help",      no_argument,       0, 'h'},
        {0, 0, 0, 0}};

    int opt;
    while ((opt = getopt_long(argc, argv, "s:i:bandh", long_opts, NULL)) != -1) {
        switch (opt) {
            case 's': size = atoi(optarg); break;
            case 'i': iters = atol(optarg); break;
            case 'b': do_bench = 1; break;
            case 'a': do_audit = 1; break;
            case 'n': do_nn = 1; break;
            case 'd': do_disasm = 1; break;
            case 'h': print_help(argv[0]); return 0;
            default:  print_help(argv[0]); return 1;
        }
    }

    if (size < 1) {
        fprintf(stderr, C_RED "error: --size must be >= 1\n" C_RESET);
        return 1;
    }
    if (iters < 1) {
        fprintf(stderr, C_RED "error: --iters must be >= 1\n" C_RESET);
        return 1;
    }

    if (!do_bench && !do_audit && !do_nn && !do_disasm) {
        print_help(argv[0]);
        return 0;
    }

    printf(C_BOLD C_MAGENTA
           "=== Cebare %s - Bare-metal AI Optimizer & Auditor ===" C_RESET "\n",
           CEBARE_VERSION);
    printf("Matrix size: %dx%d | Iterations: %ld\n", size, size, iters);

    if (do_bench)  run_benchmark(size, (size_t)iters);
    if (do_audit)  run_audit((size_t)iters);
    if (do_nn)     run_nn_demo(size);
    if (do_disasm) run_disasm();

    printf("\n" C_BOLD C_MAGENTA "=== Cebare %s Complete ===" C_RESET "\n",
           CEBARE_VERSION);
    printf("This is an early development release (%s).\n", CEBARE_VERSION);
    printf("Not intended for production or public use yet.\n");

    return 0;
}
