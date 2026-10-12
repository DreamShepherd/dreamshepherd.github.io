<script>
  import { onMount } from 'svelte';

  let { data } = $props();

  const END_ASCII = `                                        ████
                                    ████▒▒██
                                  ████  ▒▒██
                                ██▒▒  ▒▒▒▒▒▒██
                              ██▒▒██        ██
  ████                      ██▒▒██          ██
██▒▒▒▒██████                ██▒▒██      ▒▒  ████
██▒▒▒▒██    ████      ██████▒▒▒▒▒▒██    ▒▒▒▒██████████████
██▒▒    ████▒▒▒▒██████▒▒▒▒▒▒▒▒▒▒▒▒▒▒██▒▒▒▒▒▒██▒▒▒▒▒▒▒▒▒▒▒▒████
██▒▒▒▒      ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
  ██▒▒      ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████
  ██        ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
  ██▒▒    ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
  ██▒▒▒▒  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒  ██▒▒▒▒▒▒▒▒▒▒████▒▒▒▒▒▒▒▒██
    ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒    ██▒▒▒▒▒▒████▒▒▒▒▒▒▒▒▒▒▒▒██
    ████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██      ██▒▒▒▒▒▒████▒▒▒▒▒▒▒▒▒▒▒▒██
    ██▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██        ██▒▒▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
      ██▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██        ██████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
      ██▒▒██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██      ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
        ████  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒    ██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
          ██    ▒▒██████▒▒▒▒▒▒▒▒▒▒▒▒▒▒    ██▒▒  ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
          ██            ████▒▒▒▒▒▒▒▒▒▒    ██  ▒▒  ▒▒        ▒▒▒▒▒▒▒▒▒▒▒▒██
            ██                      ██  ████  ▒▒          ▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
              ██                      ██▒▒██              ▒▒  ▒▒▒▒▒▒▒▒▒▒██
                ██████████████████████▒▒▒▒██                    ▒▒▒▒▒▒██
                      ██▒▒      ██▒▒▒▒▒▒▒▒██                    ▒▒▒▒██
                      ██▒▒▒▒  ██▒▒▒▒▒▒▒▒████                  ▒▒▒▒██
                      ██▒▒▒▒▒▒██▒▒▒▒▒▒██  ██                    ██
                        ██████▒▒▒▒▒▒██    ██                ████
                              ██████      ██          ██████
                                            ██    ████
                                            ██████                          `;

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
    <div class="pt-8 flex justify-center">
      <pre class="inline-block text-left text-[9px] leading-tight font-mono whitespace-pre overflow-x-auto text-neutral-700">{END_ASCII}</pre>
    </div>
  </section>
</div>
