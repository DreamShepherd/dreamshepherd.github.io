import { useState, useEffect } from "react";
import Link from "next/link";
import { useRouter } from "next/router";

const links = [
  { name: "dreaming", href: "/dreaming" },
  { name: "resume", href: "/resume" },
  { name: "projects", href: "/doge_intentional_404_for_the_nosy" },
];

export default function Header() {
  const [pageTitle, setPageTitle] = useState(null);
  const router = useRouter();
  const isDreaming = router.pathname.startsWith("/dreaming");
  const isDogeLike = router.pathname === "/404";
  const linkjs = links.map((link, index) => (
    <Link
      href={link.href}
      key={index}
      scroll={false}
      className={`hover:text-blue-300 transition-colors ${
        router.pathname === link.href ? "text-black" : ""
      }`}
    >
      {link.name}
    </Link>
  ));

  useEffect(() => {
    if (isDogeLike) {
      setPageTitle(null);
      return;
    }

    const link = links.find(({ href }) => href === router.pathname);
    if (link) {
      setPageTitle(link.name.charAt(0).toUpperCase() + link.name.slice(1));
    } else {
      setPageTitle(null);
    }
  }, [router.pathname, isDogeLike]);

  if (isDreaming) return null;

  return (
    <div className="layout">
      <header className=" flex justify-between items-start text-neutral-400">
        <h1 className="font-bold text-black text-2xl mb-6">
          {isDogeLike ? (
            <Link scroll={false} href="/">
              Nosy one, aren't ya? 🐕
            </Link>
          ) : (
            <>
              <Link scroll={false} href="/">
                Bryant Har{" "}
              </Link>
              {pageTitle ? (
                <span className="page-title">
                  <span className="text-neutral-400">—</span> {pageTitle}
                </span>
              ) : null}
            </>
          )}
        </h1>
        <div className="flex items-start justify-end space-x-6 text-lg">
          {linkjs}
        </div>
      </header>
    </div>
  );
}
