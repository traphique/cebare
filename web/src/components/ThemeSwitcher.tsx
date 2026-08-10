"use client";

import { THEMES } from "@/lib/themes";
import { useTheme } from "@/components/ThemeProvider";

export function ThemeSwitcher() {
  const { theme, setTheme } = useTheme();

  return (
    <div
      className="flex items-center gap-1 border border-line bg-surface p-1"
      role="group"
      aria-label="Color theme"
    >
      {THEMES.map((t) => {
        const active = theme === t.id;
        return (
          <button
            key={t.id}
            type="button"
            title={t.blurb}
            aria-pressed={active}
            onClick={() => setTheme(t.id)}
            className={`px-2.5 py-1.5 font-mono text-[11px] uppercase tracking-[0.14em] transition-colors ${
              active
                ? "bg-accent text-bg"
                : "text-fg-muted hover:text-fg"
            }`}
          >
            {t.label}
          </button>
        );
      })}
    </div>
  );
}
