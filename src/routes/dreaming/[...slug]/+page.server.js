import { error } from '@sveltejs/kit';
import { getDreamEntryBySlug } from '#lib/content.js';

export function load({ params }) {
  const slug = params.slug;
  const entry = getDreamEntryBySlug(slug);
  if (!entry) error(404, 'Not found');
  return { entry };
}
