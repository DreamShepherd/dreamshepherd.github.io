import { getDreamEntries } from '#lib/content.js';

export function load() {
  return { entries: getDreamEntries() };
}
