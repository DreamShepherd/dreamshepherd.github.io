<script>
  let { data } = $props();
</script>

<div class="grid gap-y-4">
  {#each data as item (item.title)}
    <div class="block -mx-3 px-3 py-2 hover:bg-neutral-100 transition-colors">
      <div class="flex flex-col sm:flex-row sm:items-end mb-1.5">
        <div class="text-lg text-black font-normal">{item.title}</div>
        <div class="sm:ml-auto mb-0.5 text-neutral-500">{item.date}</div>
      </div>
      <div class="text-lg leading-snug font-serif italic pb-2">{item.desc}</div>
      <ul class="pl-8 space-y-0.5 text-black font-light">
        {#each item.notes ?? [] as line, i (i)}
          <li>- {line}</li>
        {/each}
      </ul>
    </div>
  {/each}
</div>
