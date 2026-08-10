const modules = [
  { file: "matrix.c", role: "naive_gemm · optimized_gemm · aligned alloc" },
  { file: "bench.c", role: "rdtsc · now_ns · stats · run_benchmark" },
  { file: "audit.c", role: "timing side-channel measurement" },
  { file: "nn.c", role: "2-layer forward pass + ReLU" },
  { file: "disasm.c", role: "objdump self-introspection" },
  { file: "asm_demo.c", role: "extended inline assembly" },
  { file: "main.c", role: "CLI orchestration only" },
];

export function Architecture() {
  return (
    <section id="architecture" className="border-t border-line">
      <div className="mx-auto max-w-6xl px-5 py-24 md:px-8">
        <p className="font-mono text-[11px] uppercase tracking-[0.24em] text-accent">
          Layout
        </p>
        <h2 className="mt-3 font-display text-4xl font-bold tracking-tight md:text-5xl">
          Modular C, not a monolith
        </h2>
        <p className="mt-4 max-w-xl text-fg-muted">
          Public APIs live in <code className="font-mono text-fg">include/cebare/</code>.
          One translation unit per concern. Tests link the library and prove the
          AVX-512 path against the naive reference.
        </p>

        <div className="mt-14 overflow-x-auto">
          <table className="w-full min-w-[520px] border-collapse text-left">
            <thead>
              <tr className="border-b border-line font-mono text-[11px] uppercase tracking-[0.16em] text-fg-muted">
                <th className="py-3 pr-6 font-normal">Module</th>
                <th className="py-3 font-normal">Responsibility</th>
              </tr>
            </thead>
            <tbody>
              {modules.map((m) => (
                <tr key={m.file} className="border-b border-line/70">
                  <td className="py-4 pr-6 font-mono text-sm text-accent">
                    src/{m.file}
                  </td>
                  <td className="py-4 text-sm text-fg-muted">{m.role}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </div>
    </section>
  );
}
