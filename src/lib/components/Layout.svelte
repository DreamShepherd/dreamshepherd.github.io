<script>
  import Header from './Header.svelte';
  import { page } from '$app/state';

  let { children } = $props();
  let path = $derived(page.url.pathname);
  let isDreaming = $derived(path.startsWith('/dreaming'));
</script>

<div class={isDreaming ? 'pt-6 pb-12 sm:pb-24 md:pb-32' : 'margin'} id="top">
  {#if isDreaming}
    <div class="layout sticky top-0 z-20 bg-white/90 backdrop-blur-sm pb-4">
      <h1 class="text-black text-2xl md:text-3xl font-semibold my-1 pt-2">
        <a href="/dreaming">Dreaming ↔ Butterflies</a>
      </h1>
      <p
        class="whitespace-nowrap overflow-hidden text-xl leading-none opacity-40 select-none pt-2"
        aria-hidden="true"
      >
        oh, 🐫, oh 🐫, oh, 🐫, oh 🐫, oh, 🐫, oh 🐫, oh, 🐫, oh 🐫, oh, 🐫, oh 🐫
      </p>
    </div>
  {:else}
    <Header />
  {/if}

  {@render children()}

  <section class="layout">
    <ul class="text-sm font-light mt-8 space-y-1.5">
      <li>
        Email:
        <a class="link" href="mailto:bhar@bryanthar.com">bhar@bryanthar.com</a>
      </li>
      <li>
        Github:
        <a class="link" href="https://github.com/dreamShepherd">@DreamShepherd</a>
      </li>
      <li>
        Linkedin:
        <a class="link" href="https://www.linkedin.com/in/bryant-har/">@bryant-har</a>
      </li>
    </ul>
  </section>
</div>

<div class="flex justify-end pb-8 pr-16">
  <a href="#top" aria-label="back to top">
    <svg
      xmlns="http://www.w3.org/2000/svg"
      width="24"
      height="24"
      viewBox="0 0 24 24"
      fill="none"
      stroke="currentColor"
      stroke-width="2"
      stroke-linecap="round"
      stroke-linejoin="round"
    >
      <path d="M12 19V5" />
      <path d="m5 12 7-7 7 7" />
    </svg>
  </a>
</div>
<div class="text-xs opacity-50 pb-2 font-sans">
  <div class="flex place-content-center space-x-0.5 items-center">
    <p class="text-xs">Bryant Har</p>
  </div>
</div>
