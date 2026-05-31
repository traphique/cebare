# Cebare

**Bare-metal AI optimization & reverse engineering toolkit** (v0.0.1)  
Written in C and x86_64 assembly. Built to demonstrate elite-level systems mastery.

> **Early Development Release (v0.0.1)** — This is a personal portfolio project in active development. Not intended for production use or public distribution yet. Use at your own risk for learning and experimentation only.

## What is Cebare?

Cebare is a fully functional, self-contained command-line tool that showcases advanced low-level techniques used in modern AI systems:

- High-performance AVX-512 matrix multiplication (the core operation behind every large language model)
- Statistical performance benchmarking with mean, standard deviation, min/max
- Tiny neural network forward-pass simulation (real transformer-style computation)
- Self-disassembly and reverse engineering of its own binary
- Timing side-channel analysis (defensive cybersecurity)
- Pure inline assembly examples with correct extended syntax

It is both a practical demonstration of performance engineering and a powerful educational artifact for anyone serious about systems programming, AI infrastructure, and security.

## Why Cebare Stands Out

Most engineers work at the framework or Python level. Cebare operates at the **metal** — where every cycle and every instruction matters.

This project demonstrates:
- Deep mastery of C and hand-tuned x86_64 assembly
- Understanding of modern CPU microarchitecture (AVX-512, vectorization, memory alignment)
- Performance engineering best practices (statistical benchmarking)
- Reverse engineering techniques (self-disassembly, instruction analysis)
- Defensive security awareness (constant-time code, side-channel resistance)
- Direct relevance to real AI workloads

These are exactly the rare, high-leverage skills that power frontier AI systems at companies like xAI, OpenAI, Anthropic, and NVIDIA.

## Features

- Clean, professional CLI with full options
- Real AVX-512 intrinsics + pure inline assembly
- Rigorous statistical timing over thousands of iterations
- Self-introspection via objdump parsing
- Colorful, informative terminal output
- Zero external dependencies

## Quick Start

```bash
make           # build ./cebare
make test      # build and run the correctness/unit test suite
./cebare --help
./cebare --size 16 --iters 100000 --benchmark --audit --nn-demo --disasm
```

**On a modern Intel or AMD CPU you will see:**
- 5–12× speedup over naive C
- Live disassembly highlighting `vfmadd132pd`, `vbroadcastsd zmm*`, etc.
- Clear demonstration of timing side-channels
- Working 2-layer neural network forward pass

### Mock Example Output (v0.0.1)

```bash
$ ./cebare --size 8 --iters 50000 --benchmark --audit --nn-demo
=== Cebare v0.0.1 - Bare-metal AI Optimizer & Auditor ===
Matrix size: 8×8 | Iterations: 50000

=== PERFORMANCE BENCHMARK ===
Naive C:     mean 45230 ns  (std 1240, min 43100, max 48900)
AVX-512 Opt: mean 4120 ns   (std 180,  min 3890,  max 4510)
>>> SPEEDUP: 10.98x
Correctness: PASS

=== DEFENSIVE CYBER: Timing Side-Channel Audit ===
Short path (bit=0) avg: 1240 cycles
Long  path (bit=1) avg: 9870 cycles
>>> TIMING DIFFERENCE DETECTED: 8630 cycles (8.0x slower)

=== TINY NEURAL NET FORWARD PASS DEMO ===
2-layer forward pass completed in 87.40 µs
Output[0][0] = 12.847291

=== Cebare v0.0.1 Complete ===
This is an early development release (v0.0.1).
Not intended for production or public use yet.
```

## Build Requirements

- Linux (native or WSL2)
- GCC with AVX support (`-mavx512f -mavx512dq` recommended)
- Standard build tools

Works on any CPU with AVX2 or better.

## Skills Demonstrated

- **C Systems Programming** — Pointers, aligned memory, performance-critical code
- **x86_64 Assembly** — Inline assembly, extended asm syntax, AVX-512 vector instructions
- **Performance Engineering** — Statistical benchmarking, micro-optimization
- **Reverse Engineering** — Binary introspection, instruction analysis
- **Cybersecurity** — Side-channel awareness and constant-time principles
- **AI Systems** — Direct implementation of core neural network primitives

## Project Structure

```
cebare/
├── include/cebare/        # Public module interfaces (headers)
│   ├── colors.h           #   ANSI colors + version
│   ├── matrix.h           #   GEMM kernels (naive + AVX-512)
│   ├── bench.h            #   timing primitives + statistics
│   ├── audit.h            #   timing side-channel audit
│   ├── nn.h               #   neural-net forward pass
│   ├── disasm.h           #   self-disassembly
│   └── asm_demo.h         #   inline-assembly example
├── src/                   # Implementation (one translation unit per module)
│   ├── matrix.c  bench.c  audit.c
│   ├── nn.c      disasm.c asm_demo.c
│   └── main.c             #   thin CLI / orchestration layer
├── tests/                 # Test suite (run with `make test`)
│   ├── test_matrix.c      #   optimized vs naive GEMM correctness
│   └── test_stats.c       #   statistics unit tests
├── Makefile               # Multi-TU build with auto dependencies + `make test`
└── README.md              # This file
```

## Future Directions

Cebare is designed to be extended. Natural next steps include:
- Real ONNX model loading and optimization
- Full taint tracking and symbolic execution
- GPU/PTX backend
- Custom hypervisor-based analysis sandbox

## License

MIT License

---

**Cebare** — Forged at the metal. Optimized for truth.

If you're evaluating this project for a role in systems, performance, AI infrastructure, or security engineering, this is the kind of work I produce when I go deep.