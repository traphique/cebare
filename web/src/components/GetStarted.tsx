const commands = [
  { cmd: "make", note: "Build the multi-TU binary" },
  { cmd: "make test", note: "Correctness + stats suite" },
  { cmd: "./cebare --help", note: "CLI surface" },
  {
    cmd: "./cebare --size 8 --iters 50000 --benchmark --audit --nn-demo --disasm",
    note: "Full demo pass",
  },
];

export function GetStarted() {
  return (
    <section id="start" className="border-t border-line bg-bg-elevated">
      <div className="mx-auto max-w-6xl px-5 py-24 md:px-8">
        <p className="font-mono text-[11px] uppercase tracking-[0.24em] text-accent">
          Quick start
        </p>
        <h2 className="mt-3 font-display text-4xl font-bold tracking-tight md:text-5xl">
          Build it. Prove it.
        </h2>
        <p className="mt-4 max-w-xl text-fg-muted">
          Targets x86-64 Linux/WSL with GCC and AVX-512. Requires{" "}
          <code className="font-mono text-fg">-mavx512f -mavx512dq</code>.
        </p>

        <ol className="mt-12 space-y-4">
          {commands.map((c, i) => (
            <li
              key={c.cmd}
              className="grid gap-2 border border-line bg-surface px-5 py-4 md:grid-cols-[auto_1fr_auto] md:items-center md:gap-6"
            >
              <span className="font-mono text-[11px] text-fg-muted">
                {String(i + 1).padStart(2, "0")}
              </span>
              <code className="overflow-x-auto font-mono text-sm text-fg md:text-[15px]">
                {c.cmd}
              </code>
              <span className="font-mono text-[11px] uppercase tracking-[0.12em] text-fg-muted">
                {c.note}
              </span>
            </li>
          ))}
        </ol>

        <p className="mt-10 text-sm text-fg-muted">
          Source on{" "}
          <a
            href="https://github.com/traphique/cebare"
            className="text-accent underline-offset-4 hover:underline"
            target="_blank"
            rel="noreferrer"
          >
            github.com/traphique/cebare
          </a>
          .
        </p>
      </div>
    </section>
  );
}
