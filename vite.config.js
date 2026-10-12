import { sveltekit } from '@sveltejs/kit/vite';
import { defineConfig } from 'vite';
import adapter from '@sveltejs/adapter-static';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const root = path.dirname(fileURLToPath(import.meta.url));

export default defineConfig({
  resolve: {
    alias: {
      '#lib': path.resolve(root, 'src/lib')
    }
  },
  plugins: [
    sveltekit({
      adapter: adapter({ fallback: '404.html' }),
      files: {
        assets: 'public'
      }
    })
  ]
});
