import { useState, useEffect } from "react";
import Link from "next/link";
import { useRouter } from "next/router";

const links = [
  // { name: "projects", href: "/projects" },
  // { name: "resume", href: "/resume" },
  // { name: "arts", href: "/arts" },
];

export default function Header() {
  const [pageTitle, setPageTitle] = useState(null);
  const router = useRouter();
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
    const link = links.find(({ href }) => href === router.pathname);
    if (link) {
      setPageTitle(link.name.charAt(0).toUpperCase() + link.name.slice(1));
    } else {
      setPageTitle(null);
    }
  }, [router.pathname]);

  return (
    <div className="layout">
      <header className=" flex justify-between items-start text-neutral-400">
        <h1 className="font-bold text-black text-2xl mb-6">
          <Link scroll={false} href="/">
            Bryant Har{" "}
          </Link>
          {pageTitle ? (
            <span className="page-title">
              <span className="text-neutral-400">—</span> {pageTitle}
            </span>
          ) : null}
        </h1>
        <div className="flex items-start justify-end space-x-6 text-lg">
          {linkjs}
        </div>
      </header>
    </div>
  );
}
