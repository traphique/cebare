export type ThemeId = "oxide" | "phosphor" | "ion" | "graphite";

export type Theme = {
  id: ThemeId;
  label: string;
  blurb: string;
};

export const THEMES: Theme[] = [
  {
    id: "oxide",
    label: "Oxide",
    blurb: "Copper on charcoal — forged metal",
  },
  {
    id: "phosphor",
    label: "Phosphor",
    blurb: "CRT green — classic terminal",
  },
  {
    id: "ion",
    label: "Ion",
    blurb: "Cyan on navy — semiconductor lab",
  },
  {
    id: "graphite",
    label: "Graphite",
    blurb: "Steel on slate — precision instrument",
  },
];

export const DEFAULT_THEME: ThemeId = "oxide";
