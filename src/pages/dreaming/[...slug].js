import Link from "next/link";
import { layout } from "../../components/Layout.js";

DreamEntry.getLayout = layout;

export default function DreamEntry({ entry }) {
  if (!entry) return null;

  return (
    <div>
      <section className="section font-light">
        <p className="text-sm">
          <Link className="link" href="/dreaming">
            ← Back to Dreaming
          </Link>
        </p>
        {entry.kind === "pdf" ? (
          <div className="space-y-3">
            <h1 className="heading mb-0">{entry.title}</h1>
            <iframe
              src={entry.pdfDataUri}
              title={entry.title}
              className="w-full min-h-[80vh] border border-neutral-200 rounded"
            />
          </div>
        ) : (
          <article
            className="markdown-body space-y-2"
            dangerouslySetInnerHTML={{ __html: entry.html }}
          />
        )}
      </section>
    </div>
  );
}

export function getStaticPaths() {
  const { getDreamEntries } = require("../../lib/dreaming");
  const paths = getDreamEntries().map((entry) => ({
    params: { slug: entry.slug.split("/") },
  }));
  return { paths, fallback: false };
}

export function getStaticProps({ params }) {
  const { getDreamEntryBySlug } = require("../../lib/dreaming");
  const slug = Array.isArray(params.slug) ? params.slug.join("/") : params.slug;
  const entry = getDreamEntryBySlug(slug);
  if (!entry) return { notFound: true };

  return {
    props: {
      entry,
    },
  };
}
