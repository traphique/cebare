"use client";

import {
  createContext,
  useCallback,
  useContext,
  useEffect,
  useState,
  type ReactNode,
} from "react";
import { DEFAULT_THEME, type ThemeId } from "@/lib/themes";

type ThemeContextValue = {
  theme: ThemeId;
  setTheme: (id: ThemeId) => void;
};

const ThemeContext = createContext<ThemeContextValue | null>(null);

const STORAGE_KEY = "cebare-theme";

export function ThemeProvider({ children }: { children: ReactNode }) {
  const [theme, setThemeState] = useState<ThemeId>(DEFAULT_THEME);
  const [ready, setReady] = useState(false);

  useEffect(() => {
    const saved = window.localStorage.getItem(STORAGE_KEY) as ThemeId | null;
    if (saved && ["oxide", "phosphor", "ion", "graphite"].includes(saved)) {
      setThemeState(saved);
      document.documentElement.dataset.theme = saved;
    } else {
      document.documentElement.dataset.theme = DEFAULT_THEME;
    }
    setReady(true);
  }, []);

  const setTheme = useCallback((id: ThemeId) => {
    setThemeState(id);
    document.documentElement.dataset.theme = id;
    window.localStorage.setItem(STORAGE_KEY, id);
  }, []);

  useEffect(() => {
    if (ready) document.documentElement.dataset.theme = theme;
  }, [theme, ready]);

  return (
    <ThemeContext.Provider value={{ theme, setTheme }}>
      {children}
    </ThemeContext.Provider>
  );
}

export function useTheme() {
  const ctx = useContext(ThemeContext);
  if (!ctx) throw new Error("useTheme must be used within ThemeProvider");
  return ctx;
}
