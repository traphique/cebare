"use client";

import { ThemeSwitcher } from "@/components/ThemeSwitcher";

const links = [
  { href: "#features", label: "Features" },
  { href: "#benchmark", label: "Benchmark" },
  { href: "#architecture", label: "Architecture" },
  { href: "#start", label: "Start" },
];

export function SiteNav() {
  return (
    <header className="absolute inset-x-0 top-0 z-20">
      <nav className="mx-auto flex max-w-6xl items-center justify-between gap-4 px-5 py-5 md:px-8">
        <a
          href="#top"
          className="font-display text-lg font-bold tracking-tight text-fg"
        >
          Cebare
        </a>
        <div className="hidden items-center gap-7 md:flex">
          {links.map((l) => (
            <a
              key={l.href}
              href={l.href}
              className="font-mono text-[11px] uppercase tracking-[0.16em] text-fg-muted transition-colors hover:text-fg"
            >
              {l.label}
            </a>
          ))}
        </div>
        <ThemeSwitcher />
      </nav>
    </header>
  );
}
