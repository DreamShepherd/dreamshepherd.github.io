<script>
  let { data } = $props();

  const isBlogFolder = (folder) => /^\d{4}-\d{2}$/.test(folder);

  const entries = $derived(data.entries ?? []);

  const blogGroups = $derived(
    Object.entries(
      entries
        .filter((e) => isBlogFolder(e.folder))
        .reduce((acc, e) => {
          (acc[e.folder] ??= []).push(e);
          return acc;
        }, {})
    )
      .map(([folder, notes]) => ({ folder, notes }))
      .sort((a, b) => b.folder.localeCompare(a.folder))
  );

  const otherGroups = $derived(
    Object.entries(
      entries
        .filter((e) => !isBlogFolder(e.folder))
        .reduce((acc, e) => {
          const top = e.folder === 'root' ? 'root' : e.folder.split('/')[0];
          (acc[top] ??= []).push(e);
          return acc;
        }, {})
    )
      .map(([folder, notes]) => ({
        folder,
        notes: notes.sort((a, b) => a.slug.localeCompare(b.slug))
      }))
      .sort((a, b) => a.folder.localeCompare(b.folder))
  );
</script>

<div>
  <section class="section font-light">
    {#if entries.length === 0}
        <p class="text-sm text-neutral-500">
          No notes yet. Add markdown files to `src/content/dreaming`.
        </p>
      {:else}
        {#each blogGroups as group (group.folder)}
          <div class="mb-6">
            <h2 class="font-medium text-black mb-2">{group.folder}</h2>
            <ul class="space-y-2">
              {#each group.notes as entry (entry.href)}
                <li>
                  <a class="link" href={entry.href} rel={entry.kind === 'markdown' ? undefined : 'external'}>{entry.title}</a>
                  {#if entry.kind !== 'markdown'}<span class="text-xs text-neutral-500"> ({entry.kind})</span>{/if}
                </li>
              {/each}
            </ul>
          </div>
        {/each}
        {#if otherGroups.length > 0}
          <div class="mb-6">
            <h2 class="font-medium text-black mb-3">others</h2>
            {#each otherGroups as group (group.folder)}
              <div class="mb-4">
                <details>
                  <summary class="text-sm text-neutral-600 mb-1 cursor-pointer">{group.folder}</summary>
                  <ul class="space-y-2">
                    {#each group.notes as entry (entry.href)}
                      <li>
                        <a class="link" href={entry.href} rel={entry.kind === 'markdown' ? undefined : 'external'}>{entry.title}</a>
                        {#if entry.kind !== 'markdown'}<span class="text-xs text-neutral-500"> ({entry.kind})</span>{/if}
                      </li>
                    {/each}
                  </ul>
                </details>
              </div>
            {/each}
          </div>
        {/if}
      {/if}
  </section>
</div>
