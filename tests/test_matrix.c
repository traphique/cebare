/*
 * Correctness tests: optimized_gemm must match naive_gemm bit-for-tolerance
 * across a range of sizes, including non-multiples of 8 (exercising the
 * AVX-512 path plus the scalar remainder).
 */
#define _GNU_SOURCE

#include "cebare/matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int check_size(int n) {
    double *A  = cb_alloc_aligned((size_t)n * n);
    double *B  = cb_alloc_aligned((size_t)n * n);
    double *Cn = cb_alloc_aligned((size_t)n * n);
    double *Co = cb_alloc_aligned((size_t)n * n);

    fill_matrix(A, n, 0x1111u + (unsigned)n);
    fill_matrix(B, n, 0x2222u + (unsigned)n);

    naive_gemm(Cn, A, B, n);
    optimized_gemm(Co, A, B, n);

    int ok = 1;
    for (int i = 0; i < n * n; i++) {
        double diff = fabs(Cn[i] - Co[i]);
        double tol = 1e-9 * (1.0 + fabs(Cn[i]));
        if (diff > tol) {
            fprintf(stderr,
                    "  n=%d index=%d naive=%.12f opt=%.12f diff=%.3e\n",
                    n, i, Cn[i], Co[i], diff);
            ok = 0;
            break;
        }
    }

    printf("  [%s] optimized_gemm vs naive_gemm, n=%d\n",
           ok ? "PASS" : "FAIL", n);

    free(A);
    free(B);
    free(Cn);
    free(Co);
    return ok;
}

int main(void) {
    const int sizes[] = {1, 2, 7, 8, 9, 15, 16, 17, 32, 33};
    int failures = 0;

    printf("test_matrix:\n");
    for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        if (!check_size(sizes[i])) failures++;
    }

    if (failures) {
        printf("test_matrix: %d FAILED\n", failures);
        return 1;
    }
    printf("test_matrix: all passed\n");
    return 0;
}
