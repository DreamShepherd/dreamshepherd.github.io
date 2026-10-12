<script>
  let { data } = $props();

  const butterfly = `      .==-.                   .-==.
       \\()\`-._  \`.   .'\`  _.-\`8()/
       (88"   ::.  \\./  .::   "88)
        \\_.'\`-::::.(#).::::-'\`._/
          \`._... .q(_)p. ..._.'
            ""-..-'|=|\`-..-""
            .""' .'|=|'. \`"".
          ,':8(o)./|=|\\.(o)8:'.
         (O :8 ::/ \\_/ \\:: 8: O)
          \\O \`::/       \\::' O/
           ""--'         \`--""

`;

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
    <div class="flex justify-center">
      <pre class="inline-block text-left text-xs leading-tight font-mono whitespace-pre text-neutral-700">{butterfly}</pre>
    </div>
    <p>
      The musings of a clever fox, and yet it's mostly bleh blah bluh...
      <br />Still, I try my best to maximize the ratio of
      <b><i>|profoundness : word count|</i></b><br />
    </p>
    <span class="whisper">
      <a class="link" href="https://biowpn.github.io/bioweapon/">psst, if you like cpp, check meowt!</a>
    </span>

    <div class="pt-4">
      {#if entries.length === 0}
        <p class="text-sm text-neutral-500">
          No notes yet. Add markdown files to `src/content/dreaming`.
        </p>
      {:else}
        {#each blogGroups as group (group.folder)}
          <div class="mb-6">
            <h2 class="font-medium text-black mb-2">{group.folder}</h2>
            <ul class="space-y-2">
              {#each group.notes as entry (entry.slug)}
                <li>
                  <a class="link" href={entry.href} rel={entry.kind === 'pdf' ? 'external' : undefined}>{entry.title}</a>
                  {#if entry.kind === 'pdf'}<span class="text-xs text-neutral-500"> (pdf)</span>{/if}
                </li>
              {/each}
            </ul>
          </div>
        {/each}
        <div class="pt-6 flex justify-center">
          <img src="/static/vulnerable.png" alt="vulnerable" class="w-72 sm:w-80 md:w-96 h-auto" />
        </div>
        <br />
        <hr class="mb-6" />
        {#if otherGroups.length > 0}
          <div class="mb-6">
            <h2 class="font-medium text-black mb-3">others</h2>
            {#each otherGroups as group (group.folder)}
              <div class="mb-4">
                <details>
                  <summary class="text-sm text-neutral-600 mb-1 cursor-pointer">{group.folder}</summary>
                  <ul class="space-y-2">
                    {#each group.notes as entry (entry.slug)}
                      <li>
                        <a class="link" href={entry.href} rel={entry.kind === 'pdf' ? 'external' : undefined}>{entry.title}</a>
                        {#if entry.kind === 'pdf'}<span class="text-xs text-neutral-500"> (pdf)</span>{/if}
                      </li>
                    {/each}
                  </ul>
                </details>
              </div>
            {/each}
          </div>
        {/if}
      {/if}
    </div>
  </section>
</div>
