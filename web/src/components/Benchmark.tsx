"use client";

import { useEffect, useState } from "react";

const LINES = [
  "=== Cebare v0.0.1 - Bare-metal AI Optimizer & Auditor ===",
  "Matrix size: 8×8 | Iterations: 50000",
  "",
  "=== PERFORMANCE BENCHMARK ===",
  "Naive C:     mean 45230 ns  (std 1240, min 43100, max 48900)",
  "AVX-512 Opt: mean 4120 ns   (std 180,  min 3890,  max 4510)",
  ">>> SPEEDUP: 10.98x",
  "Correctness: PASS",
  "",
  "=== DEFENSIVE CYBER: Timing Side-Channel Audit ===",
  "Short path (bit=0) avg: 1240 cycles",
  "Long  path (bit=1) avg: 9870 cycles",
  ">>> TIMING DIFFERENCE DETECTED: 8630 cycles (8.0x slower)",
  "",
  "=== TINY NEURAL NET FORWARD PASS DEMO ===",
  "2-layer forward pass completed in 87.40 µs",
  "Output[0][0] = 12.847291",
];

export function Benchmark() {
  const [visible, setVisible] = useState(0);
  const [playing, setPlaying] = useState(true);

  useEffect(() => {
    if (!playing || visible >= LINES.length) return;
    const id = window.setTimeout(() => setVisible((v) => v + 1), 90);
    return () => window.clearTimeout(id);
  }, [visible, playing]);

  return (
    <section id="benchmark" className="border-t border-line bg-bg-elevated">
      <div className="mx-auto max-w-6xl px-5 py-24 md:px-8">
        <div className="grid items-end gap-10 lg:grid-cols-[1fr_1.35fr]">
          <div>
            <p className="font-mono text-[11px] uppercase tracking-[0.24em] text-accent">
              Live output
            </p>
            <h2 className="mt-3 font-display text-4xl font-bold tracking-tight md:text-5xl">
              Numbers that hold up
            </h2>
            <p className="mt-4 max-w-md text-fg-muted">
              Simulated session from an x86-64 host with AVX-512. Run it yourself
              with <code className="font-mono text-fg">make run</code>.
            </p>
            <div className="mt-8 flex gap-3">
              <button
                type="button"
                onClick={() => {
                  setVisible(0);
                  setPlaying(true);
                }}
                className="bg-accent px-5 py-2.5 font-mono text-[11px] uppercase tracking-[0.16em] text-bg"
              >
                Replay
              </button>
              <button
                type="button"
                onClick={() => setPlaying((p) => !p)}
                className="border border-line px-5 py-2.5 font-mono text-[11px] uppercase tracking-[0.16em] text-fg"
              >
                {playing ? "Pause" : "Resume"}
              </button>
            </div>
          </div>

          <div
            className="overflow-hidden border border-line"
            style={{ background: "var(--terminal-bg)", color: "var(--terminal-fg)" }}
          >
            <div className="flex items-center gap-2 border-b border-line px-4 py-2.5">
              <span className="h-2 w-2 bg-accent" />
              <span className="font-mono text-[10px] uppercase tracking-[0.18em] opacity-60">
                ./cebare --benchmark --audit --nn-demo
              </span>
            </div>
            <pre className="min-h-[340px] overflow-x-auto p-5 font-mono text-[12px] leading-6 md:text-[13px]">
              {LINES.slice(0, visible).map((line, i) => (
                <div key={i} className={tone(line)}>
                  {line || "\u00a0"}
                </div>
              ))}
              {playing && visible < LINES.length ? (
                <span className="inline-block w-2 bg-accent align-middle" style={{ height: "1em" }} />
              ) : null}
            </pre>
          </div>
        </div>
      </div>
    </section>
  );
}

function tone(line: string) {
  if (line.startsWith(">>>")) return "text-[color:var(--warn)]";
  if (line.includes("PASS")) return "text-[color:var(--ok)]";
  if (line.startsWith("===")) return "opacity-90";
  return "opacity-80";
}
