<script>
  import { page } from '$app/state';

  const links = [
    { name: 'dreaming', href: '/dreaming' },
    { name: 'resume', href: '/resume' }
  ];

  let path = $derived(page.url.pathname);
  // Every 404 (including /doge_intentional_404_for_the_nosy and /404) shows the nosy header.
  const known = new Set(['/', '/creative', '/dreaming', '/projects', '/random', '/resume', '/secret']);
  let isDogeLike = $derived(!known.has(path) && !path.startsWith('/dreaming/'));
  let link = $derived(links.find((l) => l.href === path));
  let pageTitle = $derived(
    isDogeLike ? null : link ? link.name.charAt(0).toUpperCase() + link.name.slice(1) : null
  );
</script>

<div class="layout">
  <header class="flex justify-between items-start text-neutral-400">
    <h1 class="font-bold text-black text-2xl mb-6">
      {#if isDogeLike}
        <a href="/">Nosy one, aren't ya? 🐕</a>
      {:else}
        <a href="/">Bryant Har </a>
        {#if pageTitle}
          <span class="page-title">
            <span class="text-neutral-400">—</span>
            {pageTitle}
          </span>
        {/if}
      {/if}
    </h1>
    <div class="flex items-start justify-end space-x-6 text-lg">
      {#each links as l}
        <a
          href={l.href}
          class="hover:text-blue-300 transition-colors {path === l.href ? 'text-black' : ''}"
          rel={l.href.startsWith('/doge') ? 'external' : undefined}
        >{l.name}</a>
      {/each}
    </div>
  </header>
</div>
