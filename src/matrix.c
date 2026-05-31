#define _GNU_SOURCE

#include "cebare/matrix.h"
#include "cebare/colors.h"

#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

double *cb_alloc_aligned(size_t count) {
    void *p = NULL;
    if (posix_memalign(&p, 64, count * sizeof(double)) != 0 || p == NULL) {
        fprintf(stderr,
                C_RED "fatal: aligned allocation of %zu doubles failed\n" C_RESET,
                count);
        exit(EXIT_FAILURE);
    }
    return (double *)p;
}

void fill_matrix(double *m, int n, unsigned seed) {
    for (int i = 0; i < n * n; i++) {
        /* Deterministic linear-congruential pseudo-random in [-1, 1). */
        seed = seed * 1103515245u + 12345u;
        m[i] = ((double)((seed >> 16) & 0x7fff) / 16384.0) - 1.0;
    }
}

void naive_gemm(double *restrict C, const double *restrict A,
                const double *restrict B, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double acc = 0.0;
            for (int k = 0; k < n; k++) {
                acc += A[(size_t)i * n + k] * B[(size_t)k * n + j];
            }
            C[(size_t)i * n + j] = acc;
        }
    }
}

/*
 * AVX-512 microkernel: broadcast each A[i][k] into a zmm register and fuse-
 * multiply across 8-wide chunks of B's k-th row, accumulating into C's i-th
 * row. Emits vbroadcastsd + vfmadd132pd. A scalar tail handles n % 8 != 0.
 */
void optimized_gemm(double *restrict C, const double *restrict A,
                    const double *restrict B, int n) {
    for (int i = 0; i < n; i++) {
        double *crow = C + (size_t)i * n;

        int j = 0;
        for (; j + 8 <= n; j += 8) {
            _mm512_storeu_pd(crow + j, _mm512_setzero_pd());
        }
        for (; j < n; j++) {
            crow[j] = 0.0;
        }

        for (int k = 0; k < n; k++) {
            const double ascal = A[(size_t)i * n + k];
            const __m512d a = _mm512_set1_pd(ascal);
            const double *brow = B + (size_t)k * n;

            j = 0;
            for (; j + 8 <= n; j += 8) {
                __m512d bvec = _mm512_loadu_pd(brow + j);
                __m512d cvec = _mm512_loadu_pd(crow + j);
                cvec = _mm512_fmadd_pd(a, bvec, cvec);
                _mm512_storeu_pd(crow + j, cvec);
            }
            for (; j < n; j++) {
                crow[j] += ascal * brow[j];
            }
        }
    }
}
