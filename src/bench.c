#define _GNU_SOURCE

#include "cebare/bench.h"
#include "cebare/matrix.h"
#include "cebare/colors.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

stats_t compute_stats(const double *v, size_t n) {
    stats_t s = {0.0, 0.0, 0.0, 0.0};
    if (n == 0) return s;

    s.min = v[0];
    s.max = v[0];
    double sum = 0.0;
    for (size_t i = 0; i < n; i++) {
        sum += v[i];
        if (v[i] < s.min) s.min = v[i];
        if (v[i] > s.max) s.max = v[i];
    }
    s.mean = sum / (double)n;

    double acc = 0.0;
    for (size_t i = 0; i < n; i++) {
        double d = v[i] - s.mean;
        acc += d * d;
    }
    s.std = sqrt(acc / (double)n);
    return s;
}

void run_benchmark(int n, size_t iters) {
    printf("\n" C_BOLD C_CYAN "=== PERFORMANCE BENCHMARK ===" C_RESET "\n");

    double *A  = cb_alloc_aligned((size_t)n * n);
    double *B  = cb_alloc_aligned((size_t)n * n);
    double *Cn = cb_alloc_aligned((size_t)n * n);
    double *Co = cb_alloc_aligned((size_t)n * n);

    fill_matrix(A, n, 0x1234);
    fill_matrix(B, n, 0x9abc);

    double *t_naive = cb_alloc_aligned(iters);
    double *t_opt   = cb_alloc_aligned(iters);

    /* Warmup to settle caches and frequency scaling. */
    naive_gemm(Cn, A, B, n);
    optimized_gemm(Co, A, B, n);

    for (size_t it = 0; it < iters; it++) {
        uint64_t t0 = now_ns();
        naive_gemm(Cn, A, B, n);
        uint64_t t1 = now_ns();
        t_naive[it] = (double)(t1 - t0);
    }

    for (size_t it = 0; it < iters; it++) {
        uint64_t t0 = now_ns();
        optimized_gemm(Co, A, B, n);
        uint64_t t1 = now_ns();
        t_opt[it] = (double)(t1 - t0);
    }

    stats_t sn = compute_stats(t_naive, iters);
    stats_t so = compute_stats(t_opt, iters);

    printf("Naive C:     mean %.0f ns  (std %.0f, min %.0f, max %.0f)\n",
           sn.mean, sn.std, sn.min, sn.max);
    printf("AVX-512 Opt: mean %.0f ns   (std %.0f,  min %.0f,  max %.0f)\n",
           so.mean, so.std, so.min, so.max);

    double speedup = (so.mean > 0.0) ? sn.mean / so.mean : 0.0;
    printf(C_BOLD C_GREEN ">>> SPEEDUP: %.2fx" C_RESET "\n", speedup);

    int correct = 1;
    for (int i = 0; i < n * n; i++) {
        double diff = fabs(Cn[i] - Co[i]);
        double tol = 1e-6 * (1.0 + fabs(Cn[i]));
        if (diff > tol) {
            correct = 0;
            break;
        }
    }
    printf("Correctness: %s\n",
           correct ? C_GREEN "PASS" C_RESET : C_RED "FAIL" C_RESET);

    free(t_naive);
    free(t_opt);
    free(A);
    free(B);
    free(Cn);
    free(Co);
}
