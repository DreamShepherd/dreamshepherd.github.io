import Link from "next/link";
import { layout } from "../../components/Layout.js";

Secret.getLayout = layout;

const majorLinks = [
  { name: "home", href: "/" },
  { name: "projects", href: "/projects" },
  { name: "dreaming", href: "/dreaming" },
  { name: "stuff", href: "/random" },
];

export default function Secret() {
  return (
    <div className="layout pt-12">
      <section className="space-y-4 text-lg">
        <h1 className="heading">
          <span className="text-teal-600">#</span> Secret
        </h1>
        <p className="text-neutral-700">Quick links to major pages.</p>
      </section>

      <section className="pt-6">
        <div className="grid gap-3 sm:grid-cols-2">
          {majorLinks.map((item) => (
            <Link
              key={item.href}
              href={item.href}
              className="block rounded-md border border-neutral-200 px-4 py-3 text-lg hover:bg-neutral-50 transition-colors"
            >
              {item.name}
            </Link>
          ))}
        </div>
      </section>
    </div>
  );
}
