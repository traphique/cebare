#define _GNU_SOURCE

#include "cebare/nn.h"
#include "cebare/matrix.h"
#include "cebare/bench.h"
#include "cebare/colors.h"

#include <stdio.h>
#include <stdlib.h>

static void relu_inplace(double *m, int count) {
    for (int i = 0; i < count; i++) {
        if (m[i] < 0.0) m[i] = 0.0;
    }
}

void run_nn_demo(int n) {
    printf("\n" C_BOLD C_CYAN
           "=== TINY NEURAL NET FORWARD PASS DEMO ===" C_RESET "\n");

    double *X  = cb_alloc_aligned((size_t)n * n); /* input activations  */
    double *W1 = cb_alloc_aligned((size_t)n * n); /* layer-1 weights    */
    double *W2 = cb_alloc_aligned((size_t)n * n); /* layer-2 weights    */
    double *H  = cb_alloc_aligned((size_t)n * n); /* hidden activations */
    double *O  = cb_alloc_aligned((size_t)n * n); /* output             */

    fill_matrix(X, n, 0x0001);
    fill_matrix(W1, n, 0x0002);
    fill_matrix(W2, n, 0x0003);

    uint64_t t0 = now_ns();
    optimized_gemm(H, X, W1, n); /* H = X * W1   */
    relu_inplace(H, n * n);      /* H = ReLU(H)  */
    optimized_gemm(O, H, W2, n); /* O = H * W2   */
    uint64_t t1 = now_ns();

    double us = (double)(t1 - t0) / 1000.0;
    printf("2-layer forward pass completed in %.2f \302\265s\n", us);
    printf("Output[0][0] = %f\n", O[0]);

    free(X);
    free(W1);
    free(W2);
    free(H);
    free(O);
}
