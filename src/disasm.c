#define _GNU_SOURCE

#include "cebare/disasm.h"
#include "cebare/asm_demo.h"
#include "cebare/colors.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int line_has_avx512(const char *line) {
    static const char *needles[] = {
        "vfmadd", "vbroadcast", "zmm", "vmovap", "vmulpd", "vaddpd",
        "vmovup", "vxorpd", NULL};
    for (int i = 0; needles[i]; i++) {
        if (strstr(line, needles[i])) return 1;
    }
    return 0;
}

void run_disasm(void) {
    printf("\n" C_BOLD C_CYAN
           "=== SELF-DISASSEMBLY: optimized_gemm ===" C_RESET "\n");

    char self[4096];
    ssize_t len = readlink("/proc/self/exe", self, sizeof(self) - 1);
    if (len < 0) {
        printf(C_RED "  could not resolve /proc/self/exe (Linux only)\n" C_RESET);
        return;
    }
    self[len] = '\0';

    char cmd[8192];
    snprintf(cmd, sizeof(cmd), "objdump -d -M intel \"%s\" 2>/dev/null", self);

    FILE *fp = popen(cmd, "r");
    if (!fp) {
        printf(C_RED "  objdump not available; cannot disassemble\n" C_RESET);
        return;
    }

    char line[1024];
    int in_func = 0;
    int printed = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (!in_func) {
            if (strstr(line, "<optimized_gemm>:")) {
                in_func = 1;
                printf(C_BOLD "%s" C_RESET, line);
            }
            continue;
        }

        /* A blank line or a new function label ends the current function. */
        if (line[0] == '\n' || strstr(line, ">:")) break;

        if (line_has_avx512(line)) {
            printf(C_GREEN "%s" C_RESET, line);
        } else {
            printf("%s", line);
        }
        if (++printed >= 80) {
            printf("  ... (truncated)\n");
            break;
        }
    }

    pclose(fp);

    if (!in_func) {
        printf(C_YELLOW
               "  optimized_gemm not found in disassembly "
               "(stripped binary or non-x86 target?)\n" C_RESET);
    }

    printf("\n");
    inline_asm_demo();
}
