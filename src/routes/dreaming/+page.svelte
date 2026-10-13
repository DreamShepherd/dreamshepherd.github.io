<script>
  let { data } = $props();

  const isBlogFolder = (folder) => /^\d{4}-\d{2}$/.test(folder);

  // Season (Northern Hemisphere) + Chinese zodiac animal for a YYYY-MM folder.
  // Zodiac flips at Chinese New Year, so Jan/Feb folders may belong to the
  // previous year's animal (e.g. 2026-02 is still the Snake until Feb 17).
  const ZODIAC = ['🐀', '🐂', '🐯', '🐰', '🐉', '🐍', '🐴', '🐐', '🐵', '🐔', '🐶', '🐷'];
  const CNY = {
    2019: '2019-02-05', 2020: '2020-01-25', 2021: '2021-02-12', 2022: '2022-02-01',
    2023: '2023-01-22', 2024: '2024-02-10', 2025: '2025-01-29', 2026: '2026-02-17',
    2027: '2027-02-06', 2028: '2028-01-26', 2029: '2029-02-13', 2030: '2030-02-03'
  };
  const zodiac = (year) => ZODIAC[(((year - 4) % 12) + 12) % 12];
  const seasonEmoji = (month) => {
    if (month >= 3 && month <= 5) return '🌸';
    if (month >= 6 && month <= 8) return '☀️';
    if (month >= 9 && month <= 11) return '🍂';
    return '❄️';
  };
  function folderEmoji(folder) {
    const m = /^(\d{4})-(\d{2})$/.exec(folder);
    if (!m) return '';
    const year = +m[1];
    const month = +m[2];
    const cny = CNY[year];
    const animalYear = cny && `${m[1]}-${m[2]}-01` < cny ? year - 1 : year;
    return seasonEmoji(month) + zodiac(animalYear);
  }

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
      .map(([folder, notes]) => ({ folder, notes, emoji: folderEmoji(folder) }))
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
            <h2 class="font-medium text-black mb-2">{group.folder}{#if group.emoji} <span aria-hidden="true">{group.emoji}</span>{/if}</h2>
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
