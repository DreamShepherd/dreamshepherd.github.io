<script>
  import LinkList from '#lib/components/LinkList.svelte';
  import data from '#lib/data/creative.json';
</script>

<div class="pt-12">
  <section class="pb-12 layout text-lg">
    <h1 class="heading">Music and Writings</h1>
    <p>
      I do <span class="text-purple-400">creative</span> stuff sometimes.
      <span class="text-sm bg-green-100"> ¯\_(ツ)_/¯ </span>
    </p>
    <p class="pb-4 pt-2">
      I play piano and recite poetry now and again. Poetry is <em><b>neat!</b></em> <br />
      This page is mostly for personal use, but feel free to take a gander. I'll add my own work soon.
      Here's quick access to stuff I've previously played and memorized.
      <span class="whisper">(or am going to...)</span>
    </p>
  </section>

  <div class="bg-gray-900 py-12 text-neutral-200">
    <section class="layout">
      <h1 class="heading text-white">Table of Contents</h1>
      <div class="grid grid-cols-2 pl-4">
        <div>
          <h2 class="text-red-50 pb-2">Sheets</h2>
          <ul class="link text-neutral-300 pl-2">
            {#each data.scores as item (item.title)}
              <li>
                <a class="link text-neutral-300 hover:text-white hover:underline" href="#{item.title}">
                  {item.title}&nbsp;<b class="text-sm">↓</b>
                </a>
              </li>
            {/each}
          </ul>
        </div>
        <div>
          <h2 class="text-red-50 pb-2">Poetry </h2>
          <ul class="pl-2">
            {#each data.poems as item (item.title)}
              <li>
                <a class="link text-neutral-300 hover:text-white hover:underline" href="#{item.title}">
                  {item.title}&nbsp;<b class="text-sm">↓</b>
                </a>
              </li>
            {/each}
          </ul>
        </div>
      </div>
    </section>
  </div>
  <section id="sheets" class="py-12 layout">
    <h1 class="heading">Sheets</h1>
    <LinkList data={data.scores} />
  </section>
  <section id="poems" class="layout">
    <h1 class="heading">Poetry</h1>
    <p>Keeping track of poems I've memorized or like a lot</p>
    <LinkList data={data.poems} />
  </section>
</div>
