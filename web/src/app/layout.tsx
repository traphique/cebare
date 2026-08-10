import type { Metadata } from "next";
import { Syne, IBM_Plex_Sans, IBM_Plex_Mono } from "next/font/google";
import { ThemeProvider } from "@/components/ThemeProvider";
import "./globals.css";

const syne = Syne({
  variable: "--font-syne",
  subsets: ["latin"],
  weight: ["500", "600", "700", "800"],
});

const plex = IBM_Plex_Sans({
  variable: "--font-plex",
  subsets: ["latin"],
  weight: ["400", "500", "600"],
});

const plexMono = IBM_Plex_Mono({
  variable: "--font-plex-mono",
  subsets: ["latin"],
  weight: ["400", "500"],
});

export const metadata: Metadata = {
  title: "Cebare — Bare-metal AI Optimizer",
  description:
    "AVX-512 GEMM, statistical benchmarking, side-channel audits, and self-disassembly — forged in C and x86_64 assembly.",
  metadataBase: new URL("https://cebare.traphic.dev"),
  openGraph: {
    title: "Cebare — Bare-metal AI Optimizer",
    description:
      "Where every cycle and every instruction matters. Portfolio toolkit for systems, AI infrastructure, and security.",
    url: "https://cebare.traphic.dev",
    siteName: "Cebare",
    type: "website",
  },
};

const themeBoot = `
(function(){
  try {
    var t = localStorage.getItem('cebare-theme');
    if (t === 'oxide' || t === 'phosphor' || t === 'ion' || t === 'graphite') {
      document.documentElement.dataset.theme = t;
    } else {
      document.documentElement.dataset.theme = 'oxide';
    }
  } catch (e) {
    document.documentElement.dataset.theme = 'oxide';
  }
})();
`;

export default function RootLayout({ children }: LayoutProps<"/">) {
  return (
    <html
      lang="en"
      data-theme="oxide"
      className={`${syne.variable} ${plex.variable} ${plexMono.variable} h-full antialiased`}
      suppressHydrationWarning
    >
      <head>
        <script dangerouslySetInnerHTML={{ __html: themeBoot }} />
      </head>
      <body className="min-h-full bg-bg text-fg">
        <ThemeProvider>{children}</ThemeProvider>
      </body>
    </html>
  );
}
