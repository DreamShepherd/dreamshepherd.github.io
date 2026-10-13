<script>
  import Header from './Header.svelte';

  let { children } = $props();
</script>

<div class="margin" id="top">
  <Header />

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
