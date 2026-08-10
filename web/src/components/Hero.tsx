export function Hero() {
  return (
    <section
      id="top"
      className="hero-plane relative min-h-[100svh] overflow-hidden"
    >
      <div className="hero-grid absolute inset-0 opacity-60" aria-hidden />
      <div
        className="scanline absolute inset-x-0 top-0 h-24 bg-gradient-to-b from-transparent via-accent/10 to-transparent opacity-40"
        aria-hidden
      />

      {/* Full-bleed register / matrix visual plane */}
      <div
        className="pointer-events-none absolute inset-y-0 right-0 w-full md:w-[58%]"
        aria-hidden
      >
        <MetalField />
      </div>

      <div className="relative z-10 mx-auto flex min-h-[100svh] max-w-6xl flex-col justify-end px-5 pb-16 pt-28 md:justify-center md:px-8 md:pb-24 md:pt-20">
        <p className="animate-rise font-mono text-[11px] uppercase tracking-[0.28em] text-accent">
          v0.0.1 · Bare-metal AI toolkit
        </p>
        <h1 className="animate-rise-delay-1 mt-4 max-w-xl font-display text-6xl font-bold leading-[0.92] tracking-tight text-fg md:text-8xl">
          Cebare
        </h1>
        <p className="animate-rise-delay-2 mt-6 max-w-md text-lg leading-relaxed text-fg-muted md:text-xl">
          Where every cycle and every instruction matters — AVX-512 GEMM,
          side-channel audits, and self-disassembly in pure C.
        </p>
        <div className="animate-rise-delay-3 mt-10 flex flex-wrap items-center gap-4">
          <a
            href="#start"
            className="bg-accent px-6 py-3 font-mono text-xs uppercase tracking-[0.18em] text-bg transition-opacity hover:opacity-90"
          >
            Get started
          </a>
          <a
            href="#benchmark"
            className="border border-line px-6 py-3 font-mono text-xs uppercase tracking-[0.18em] text-fg transition-colors hover:border-fg"
          >
            See the numbers
          </a>
        </div>
      </div>
    </section>
  );
}

function MetalField() {
  return (
    <div className="relative h-full w-full">
      <svg
        className="absolute inset-0 h-full w-full opacity-70"
        viewBox="0 0 800 900"
        fill="none"
        xmlns="http://www.w3.org/2000/svg"
        preserveAspectRatio="xMidYMid slice"
      >
        <defs>
          <linearGradient id="wire" x1="0" y1="0" x2="1" y2="1">
            <stop offset="0%" stopColor="var(--accent)" stopOpacity="0.55" />
            <stop offset="100%" stopColor="var(--accent)" stopOpacity="0.05" />
          </linearGradient>
        </defs>
        {/* Abstract zmm / matrix lanes */}
        {Array.from({ length: 12 }).map((_, i) => (
          <g key={i} opacity={0.35 + (i % 4) * 0.1}>
            <rect
              x={40 + i * 58}
              y={80 + (i % 3) * 40}
              width={28}
              height={620 - (i % 5) * 40}
              stroke="url(#wire)"
              strokeWidth="1.2"
              fill="var(--accent)"
              fillOpacity={0.03 + (i % 3) * 0.02}
            />
            {Array.from({ length: 8 }).map((_, j) => (
              <circle
                key={j}
                cx={54 + i * 58}
                cy={120 + j * 70 + (i % 2) * 20}
                r={2.5}
                fill="var(--accent)"
                fillOpacity={0.45}
              />
            ))}
          </g>
        ))}
        <path
          d="M60 200 H740 M60 360 H740 M60 520 H740 M60 680 H740"
          stroke="var(--accent)"
          strokeOpacity="0.15"
          strokeWidth="1"
        />
      </svg>

      {/* Live equalizer bars suggesting throughput */}
      <div className="absolute bottom-[18%] left-[12%] flex h-28 items-end gap-1.5 md:left-[22%]">
        {Array.from({ length: 16 }).map((_, i) => (
          <div
            key={i}
            className="eq-bar w-1.5 bg-accent/70 md:w-2"
            style={{
              height: `${30 + ((i * 37) % 70)}%`,
              animationDelay: `${(i % 8) * 0.12}s`,
            }}
          />
        ))}
      </div>
    </div>
  );
}
