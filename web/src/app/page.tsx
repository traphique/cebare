import { SiteNav } from "@/components/SiteNav";
import { Hero } from "@/components/Hero";
import { Features } from "@/components/Features";
import { Benchmark } from "@/components/Benchmark";
import { Architecture } from "@/components/Architecture";
import { GetStarted } from "@/components/GetStarted";
import { SiteFooter } from "@/components/SiteFooter";

export default function Home() {
  return (
    <>
      <SiteNav />
      <main>
        <Hero />
        <Features />
        <Benchmark />
        <Architecture />
        <GetStarted />
      </main>
      <SiteFooter />
    </>
  );
}
