const features = [
  {
    title: "AVX-512 GEMM",
    body: "Broadcast + FMA microkernel with a scalar remainder path. Emits vbroadcastsd and vfmadd132pd you can inspect live.",
  },
  {
    title: "Statistical bench",
    body: "Mean, std, min, and max over thousands of iterations — not a single lucky timing sample.",
  },
  {
    title: "Side-channel audit",
    body: "rdtsc-timed secret-dependent paths that make timing leaks visible, not theoretical.",
  },
  {
    title: "Self-disassembly",
    body: "Resolves /proc/self/exe, runs objdump, and highlights the kernel’s AVX-512 mnemonics.",
  },
  {
    title: "Neural forward pass",
    body: "Two-layer GEMM → ReLU → GEMM demo — the same primitive behind transformer workloads.",
  },
  {
    title: "Inline assembly",
    body: "Correct GCC extended-asm with named operands, clobbers, and real zmm instructions.",
  },
];

export function Features() {
  return (
    <section id="features" className="border-t border-line">
      <div className="mx-auto max-w-6xl px-5 py-24 md:px-8">
        <p className="font-mono text-[11px] uppercase tracking-[0.24em] text-accent">
          Capabilities
        </p>
        <h2 className="mt-3 max-w-2xl font-display text-4xl font-bold tracking-tight md:text-5xl">
          Forged at the metal
        </h2>
        <p className="mt-4 max-w-xl text-fg-muted">
          One job per module. Headers expose the API; internals stay static.
          The CLI is orchestration, not a dumping ground.
        </p>

        <div className="mt-16 grid gap-x-10 gap-y-12 sm:grid-cols-2 lg:grid-cols-3">
          {features.map((f) => (
            <article key={f.title}>
              <h3 className="font-display text-xl font-semibold tracking-tight">
                {f.title}
              </h3>
              <p className="mt-3 text-[15px] leading-relaxed text-fg-muted">
                {f.body}
              </p>
            </article>
          ))}
        </div>
      </div>
    </section>
  );
}
