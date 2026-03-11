import fs from "fs";
import path from "path";

const CONTENT_DIR = path.join(process.cwd(), "src", "content", "dreaming");

function escapeHtml(text) {
  return text
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;");
}

function parseInline(text) {
  let out = escapeHtml(text);
  out = out.replace(
    /\[([^\]]+)\]\((https?:\/\/[^\s)]+)\)/g,
    '<a class="link" href="$2">$1</a>'
  );
  out = out.replace(/`([^`]+)`/g, "<code>$1</code>");
  out = out.replace(/\*\*([^*]+)\*\*/g, "<strong>$1</strong>");
  out = out.replace(/\*([^*]+)\*/g, "<em>$1</em>");
  return out;
}

export function parseMarkdown(markdown) {
  const lines = markdown.replace(/\r\n/g, "\n").split("\n");
  const html = [];
  let inCodeBlock = false;
  let inUl = false;
  let inOl = false;
  let inBlockquote = false;
  let blockquoteLines = [];

  const closeLists = () => {
    if (inUl) {
      html.push("</ul>");
      inUl = false;
    }
    if (inOl) {
      html.push("</ol>");
      inOl = false;
    }
  };

  const flushBlockquote = () => {
    if (!inBlockquote) return;
    const content = blockquoteLines
      .map((line) => parseInline(line))
      .join("<br />");
    html.push(`<blockquote>${content}</blockquote>`);
    inBlockquote = false;
    blockquoteLines = [];
  };

  for (const line of lines) {
    const trimmed = line.trim();

    if (trimmed.startsWith("```")) {
      flushBlockquote();
      closeLists();
      if (!inCodeBlock) {
        html.push("<pre><code>");
      } else {
        html.push("</code></pre>");
      }
      inCodeBlock = !inCodeBlock;
      continue;
    }

    if (inCodeBlock) {
      html.push(`${escapeHtml(line)}\n`);
      continue;
    }

    if (!trimmed) {
      flushBlockquote();
      closeLists();
      continue;
    }

    const headingMatch = trimmed.match(/^(#{1,6})\s+(.+)$/);
    if (headingMatch) {
      flushBlockquote();
      closeLists();
      const level = headingMatch[1].length;
      html.push(`<h${level}>${parseInline(headingMatch[2])}</h${level}>`);
      continue;
    }

    const ulMatch = trimmed.match(/^[-*]\s+(.+)$/);
    if (ulMatch) {
      flushBlockquote();
      if (!inUl) {
        closeLists();
        html.push("<ul>");
        inUl = true;
      }
      html.push(`<li>${parseInline(ulMatch[1])}</li>`);
      continue;
    }

    const olMatch = trimmed.match(/^\d+\.\s+(.+)$/);
    if (olMatch) {
      flushBlockquote();
      if (!inOl) {
        closeLists();
        html.push("<ol>");
        inOl = true;
      }
      html.push(`<li>${parseInline(olMatch[1])}</li>`);
      continue;
    }

    if (trimmed.startsWith("> ")) {
      closeLists();
      inBlockquote = true;
      blockquoteLines.push(trimmed.slice(2));
      continue;
    }

    if (trimmed === "---") {
      flushBlockquote();
      closeLists();
      html.push("<hr />");
      continue;
    }

    flushBlockquote();
    closeLists();
    html.push(`<p>${parseInline(trimmed)}</p>`);
  }

  flushBlockquote();
  closeLists();
  if (inCodeBlock) html.push("</code></pre>");

  return html.join("\n");
}

function getTitle(markdown, fallback) {
  const firstHeading = markdown.match(/^#\s+(.+)$/m);
  if (firstHeading) return firstHeading[1].trim();
  return fallback;
}

function getContentFiles(dir) {
  const files = [];
  const entries = fs.readdirSync(dir, { withFileTypes: true });

  for (const entry of entries) {
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
  if (!fs.existsSync(CONTENT_DIR)) return [];

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
      const title =
        ext === ".md"
          ? getTitle(fs.readFileSync(fullPath, "utf8"), fallbackTitle)
          : fallbackTitle;
      return {
        slug,
        folder,
        kind: ext === ".md" ? "markdown" : "pdf",
        title,
        updatedAt: stat.mtime.toISOString(),
      };
    })
    .sort((a, b) => (a.updatedAt < b.updatedAt ? 1 : -1));
}

export function getDreamEntryBySlug(slug) {
  const markdownPath = path.join(CONTENT_DIR, `${slug}.md`);
  if (fs.existsSync(markdownPath)) {
    const markdown = fs.readFileSync(markdownPath, "utf8");
    const fallbackTitle = slug.split("/").at(-1) ?? slug;
    return {
      slug,
      kind: "markdown",
      title: getTitle(markdown, fallbackTitle),
      html: parseMarkdown(markdown),
    };
  }

  const pdfPath = path.join(CONTENT_DIR, `${slug}.pdf`);
  if (fs.existsSync(pdfPath)) {
    const fallbackTitle = slug.split("/").at(-1) ?? slug;
    const pdfBase64 = fs.readFileSync(pdfPath).toString("base64");
    return {
      slug,
      kind: "pdf",
      title: fallbackTitle,
      pdfDataUri: `data:application/pdf;base64,${pdfBase64}`,
    };
  }

  return null;
}
