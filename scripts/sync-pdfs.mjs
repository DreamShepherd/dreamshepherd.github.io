#!/usr/bin/env node
// Copies the PDFs referenced by the dreaming index into the static assets dir.
// Markdown is rendered to HTML at build time, so only PDFs need to ship.
import fs from 'node:fs';
import path from 'node:path';

const SRC = path.join(process.cwd(), 'src', 'content', 'dreaming');
const DEST = path.join(process.cwd(), 'public', 'content', 'dreaming');

if (fs.existsSync(DEST)) fs.rmSync(DEST, { recursive: true, force: true });

let count = 0;

function walk(dir) {
  if (!fs.existsSync(dir)) return;
  for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
    const from = path.join(dir, entry.name);
    if (entry.isDirectory()) {
      walk(from);
      continue;
    }
    if (path.extname(entry.name).toLowerCase() !== '.pdf') continue;
    const to = path.join(DEST, path.relative(SRC, from));
    fs.mkdirSync(path.dirname(to), { recursive: true });
    fs.copyFileSync(from, to);
    count++;
  }
}

walk(SRC);
console.log(`synced ${count} pdfs -> public/content/dreaming`);
