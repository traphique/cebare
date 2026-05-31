/* Unit tests for compute_stats over a known sample. */
#define _GNU_SOURCE

#include "cebare/bench.h"

#include <stdio.h>
#include <math.h>

static int approx(double a, double b) {
    return fabs(a - b) < 1e-9;
}

int main(void) {
    printf("test_stats:\n");

    /* {2, 4, 4, 4, 5, 5, 7, 9}: mean 5, population std 2. */
    double v[] = {2, 4, 4, 4, 5, 5, 7, 9};
    stats_t s = compute_stats(v, sizeof(v) / sizeof(v[0]));

    int ok = 1;
    if (!approx(s.mean, 5.0)) { printf("  [FAIL] mean=%.12f want 5\n", s.mean); ok = 0; }
    if (!approx(s.std, 2.0))  { printf("  [FAIL] std=%.12f want 2\n", s.std);  ok = 0; }
    if (!approx(s.min, 2.0))  { printf("  [FAIL] min=%.12f want 2\n", s.min);  ok = 0; }
    if (!approx(s.max, 9.0))  { printf("  [FAIL] max=%.12f want 9\n", s.max);  ok = 0; }

    /* Empty input must be well-defined (all zero). */
    stats_t e = compute_stats(v, 0);
    if (!(e.mean == 0.0 && e.std == 0.0 && e.min == 0.0 && e.max == 0.0)) {
        printf("  [FAIL] empty input not zeroed\n");
        ok = 0;
    }

    if (!ok) {
        printf("test_stats: FAILED\n");
        return 1;
    }
    printf("  [PASS] mean/std/min/max\n");
    printf("test_stats: all passed\n");
    return 0;
}
