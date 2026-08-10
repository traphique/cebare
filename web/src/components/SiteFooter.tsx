export function SiteFooter() {
  return (
    <footer className="border-t border-line">
      <div className="mx-auto flex max-w-6xl flex-col gap-4 px-5 py-10 md:flex-row md:items-center md:justify-between md:px-8">
        <div>
          <p className="font-display text-lg font-bold tracking-tight">Cebare</p>
          <p className="mt-1 font-mono text-[11px] uppercase tracking-[0.16em] text-fg-muted">
            Forged at the metal. Optimized for truth.
          </p>
        </div>
        <p className="font-mono text-[11px] text-fg-muted">
          MIT · v0.0.1 · cebare.traphic.dev
        </p>
      </div>
    </footer>
  );
}
