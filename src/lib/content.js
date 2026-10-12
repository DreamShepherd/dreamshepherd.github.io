import fs from "node:fs";
import path from "node:path";
import { parseMarkdown, getTitle } from "./markdown.js";

const CONTENT_DIR = path.join(process.cwd(), "src", "content", "dreaming");

function encodePath(slug) {
  return slug.split("/").map(encodeURIComponent).join("/");
}

function getContentFiles(dir) {
  const files = [];
  if (!fs.existsSync(dir)) return files;
  for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
    const fullPath = path.join(dir, entry.name);
    if (entry.isDirectory()) {
      files.push(...getContentFiles(fullPath));
      continue;
    }
    const ext = path.extname(entry.name).toLowerCase();
    if (entry.isFile() && (ext === ".md" || ext === ".pdf")) {
      files.push(fullPath);
    }
  }
  return files;
}

export function getDreamEntries() {
  return getContentFiles(CONTENT_DIR)
    .map((fullPath) => {
      const relPath = path
        .relative(CONTENT_DIR, fullPath)
        .replaceAll(path.sep, "/");
      const ext = path.extname(relPath).toLowerCase();
      const slug = relPath.slice(0, -ext.length);
      const folder = path.dirname(relPath).replaceAll(path.sep, "/");
      const stat = fs.statSync(fullPath);
      const fallbackTitle = slug.split("/").at(-1) ?? slug;
      const isMarkdown = ext === ".md";
      return {
        slug,
        folder: folder === "." ? "root" : folder,
        kind: isMarkdown ? "markdown" : "pdf",
        title: isMarkdown
          ? getTitle(fs.readFileSync(fullPath, "utf8"), fallbackTitle)
          : fallbackTitle,
        href: isMarkdown
          ? `/dreaming/${encodePath(slug)}`
          : `/content/dreaming/${encodePath(slug)}${ext}`,
        updatedAt: stat.mtime.toISOString()
      };
    })
    .sort((a, b) => (a.updatedAt < b.updatedAt ? 1 : -1));
}

export function getDreamEntryBySlug(slug) {
  const markdownPath = path.join(CONTENT_DIR, `${slug}.md`);
  if (!fs.existsSync(markdownPath)) return null;
  const markdown = fs.readFileSync(markdownPath, "utf8");
  const fallbackTitle = slug.split("/").at(-1) ?? slug;
  return {
    slug,
    kind: "markdown",
    title: getTitle(markdown, fallbackTitle),
    html: parseMarkdown(markdown)
  };
}
