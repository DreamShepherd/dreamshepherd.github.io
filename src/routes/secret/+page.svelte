<script>
  const majorLinks = [
    { name: 'home', href: '/' },
    { name: 'dreaming', href: '/dreaming' },
    { name: 'resume', href: '/resume' },
    { name: 'projects (actual page)', href: '/projects' },
    { name: 'projects (tab route)', href: '/doge_intentional_404_for_the_nosy' },
    { name: 'stuff', href: '/random' }
  ];
</script>

<div class="layout pt-12">
  <section class="space-y-4 text-lg">
    <h1 class="heading"><span class="text-teal-600">#</span> Secret</h1>
    <p class="text-neutral-700">Quick links to major pages.</p>
  </section>

  <section class="pt-6">
    <div class="grid gap-3 sm:grid-cols-2">
      {#each majorLinks as item (item.href)}
        <a
          href={item.href}
          class="block rounded-md border border-neutral-200 px-4 py-3 text-lg hover:bg-neutral-50 transition-colors"
          rel={item.href.startsWith('/doge') ? 'external' : undefined}
        >
          {item.name}
        </a>
      {/each}
    </div>
  </section>
</div>
