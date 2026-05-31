#define _GNU_SOURCE

#include "cebare/audit.h"
#include "cebare/bench.h"
#include "cebare/colors.h"

#include <stdio.h>
#include <math.h>

/*
 * Deliberately data-dependent: the work performed depends on the secret `bit`.
 * This is precisely the pattern that leaks secrets through timing in code that
 * is not constant-time.
 */
static double secret_dependent_work(int bit, double seed) {
    int rounds = bit ? 8000 : 1000;
    double acc = seed;
    for (int i = 0; i < rounds; i++) {
        acc += sin((double)i * 0.001) * 1e-6;
    }
    return acc;
}

void run_audit(size_t iters) {
    printf("\n" C_BOLD C_CYAN
           "=== DEFENSIVE CYBER: Timing Side-Channel Audit ===" C_RESET "\n");

    volatile double sink = 0.0;
    size_t trials = iters / 10 + 1; /* cycle counts are large; fewer trials */

    uint64_t total0 = 0, total1 = 0;

    sink += secret_dependent_work(0, 1.0);
    sink += secret_dependent_work(1, 1.0);

    for (size_t t = 0; t < trials; t++) {
        uint64_t c0 = rdtsc();
        sink += secret_dependent_work(0, (double)t);
        uint64_t c1 = rdtsc();
        total0 += (c1 - c0);

        uint64_t c2 = rdtsc();
        sink += secret_dependent_work(1, (double)t);
        uint64_t c3 = rdtsc();
        total1 += (c3 - c2);
    }

    double avg0 = (double)total0 / (double)trials;
    double avg1 = (double)total1 / (double)trials;
    double diff = avg1 - avg0;
    double ratio = (avg0 > 0.0) ? avg1 / avg0 : 0.0;

    printf("Short path (bit=0) avg: %.0f cycles\n", avg0);
    printf("Long  path (bit=1) avg: %.0f cycles\n", avg1);
    printf(C_BOLD C_YELLOW
           ">>> TIMING DIFFERENCE DETECTED: %.0f cycles (%.1fx slower)" C_RESET "\n",
           diff, ratio);

    /* Prevent the optimizer from discarding the measured work. */
    if (sink == 1234567.0) printf(" ");
}
