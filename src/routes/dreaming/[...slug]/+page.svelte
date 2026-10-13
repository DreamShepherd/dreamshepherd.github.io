<script>
  import { onMount } from 'svelte';

  let { data } = $props();

  onMount(() => {
    const target = document.getElementById('dream-article');
    if (!target) return;

    let tries = 0;
    const render = () => {
      const autoRender = window.renderMathInElement;
      const katex = window.katex;
      if (!autoRender || !katex) {
        if (++tries < 20) setTimeout(render, 100);
        return;
      }
      try {
        autoRender(target, {
          delimiters: [
            { left: '$$', right: '$$', display: true },
            { left: '$', right: '$', display: false }
          ],
          throwOnError: false
        });
      } catch {
        // ignore katex failures
      }
    };
    render();
  });
</script>

<div>
  <section class="section font-light">
    <p class="text-sm">
      <a class="link" href="/dreaming">← Back to Dreaming</a>
    </p>
    <article id="dream-article" class="markdown-body space-y-2">
      {@html data.entry.html}
    </article>
  </section>
</div>
