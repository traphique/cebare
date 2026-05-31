#define _GNU_SOURCE

#include "cebare/asm_demo.h"
#include "cebare/colors.h"

#include <stdio.h>

/*
 * Pure inline-assembly demonstration with correct GCC extended-asm syntax.
 *
 * out[0..7] = in[0..7] + scalar, via a broadcast + packed add and an aligned
 * store. Exercises vbroadcastsd, vaddpd, and vmovapd with proper input/output/
 * clobber constraints. Clobbering xmm0/xmm1 marks the full zmm0/zmm1 dirty.
 */
void inline_asm_demo(void) {
    double in[8]  __attribute__((aligned(64))) = {1, 2, 3, 4, 5, 6, 7, 8};
    double out[8] __attribute__((aligned(64))) = {0};
    double scalar = 10.0;

    __asm__ volatile(
        "vbroadcastsd %[s], %%zmm0\n\t"
        "vaddpd (%[in]), %%zmm0, %%zmm1\n\t"
        "vmovapd %%zmm1, (%[out])\n\t"
        :
        : [s] "m"(scalar), [in] "r"(in), [out] "r"(out)
        : "xmm0", "xmm1", "memory");

    printf("  " C_BOLD "Pure inline asm (vbroadcastsd + vaddpd):" C_RESET "\n");
    printf("    in     = [%.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f]\n",
           in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7]);
    printf("    scalar = %.0f (broadcast)\n", scalar);
    printf("    out    = [%.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f]\n",
           out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7]);
}
