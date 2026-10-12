<script>
  let { data } = $props();
</script>

<div class="grid gap-y-4">
  {#each data as item (item.title)}
    <a
      id={item.title}
      href={item.link}
      class="block -mx-3 px-3 py-2 hover:bg-neutral-100 transition-colors"
      target="_blank"
      rel="noreferrer"
    >
      <div class="flex flex-col sm:flex-row sm:items-end mb-1.5">
        <div class="text-lg text-black font-normal">
          {item.title}&nbsp;
          <svg
            class="inline text-neutral-400"
            width="18"
            height="18"
            viewBox="0 0 24 24"
            fill="none"
            stroke="currentColor"
            stroke-width="2"
            stroke-linecap="round"
            stroke-linejoin="round"
          >
            <path d="M7 7h10v10" />
            <path d="M7 17 17 7" />
          </svg>
        </div>
        <div class="sm:ml-auto mb-0.5 text-neutral-500">{item.date}</div>
      </div>
      <div class="text-lg leading-snug font-serif italic">{item.summary}</div>
    </a>
  {/each}
</div>
