<script>
  import data from '#lib/data/projects.json';

  let projs = $derived(data.projects);
</script>

<div class="pt-12">
  <section class="pb-12 layout text-lg">
    <h1 class="heading">Projects</h1>
    <p class="pb-4 pt-2">
      A quick overview of my github for personal reference (most of these are private repos). Much of
      it is technical projects, but I also enjoy the designing and ux sometimes. Reach out
      <a class="link" href="mailto:bjh254@cornell.edu">here</a> if you'd like to design something
      together! :D
    </p>
  </section>

  <div class="bg-gray-900 py-12 text-neutral-200">
    <section class="layout">
      <h1 class="heading text-white">Table of Contents</h1>
      <div class="grid grid-cols-2 gap-1 pl-4">
        {#each projs as item (item.title)}
          <a class="link text-neutral-300 hover:text-white hover:underline" href="#{item.title}">
            {item.title}&nbsp;<b class="text-sm">↓</b>
          </a>
        {/each}
        <a class="link text-neutral-300 hover:text-white hover:underline" href="#research">
          Research&nbsp;<b class="text-sm">↓</b>
        </a>
      </div>
    </section>
  </div>
  <section class="layout pt-12">
    <div>
      <img loading="eager" class="mx-auto" width="400" src="/static/arm.jpg" alt="robotic arm" />
      <p class="text-center font-serif mx-auto text-black font-bold pt-2 pb-6">
        Franka Emika Panda robotic arm in the lab
      </p>
    </div>
    <div class="grid gap-y-4">
      {#each projs as item (item.title)}
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
          <div class="text-lg leading-snug font-light italic">{item.summary}</div>
        </a>
      {/each}
    </div>
  </section>
  <section id="research" class="layout py-12 text-lg space-y-4">
    <h1 class="heading"><span class="text-teal-600">#</span> Research</h1>
    <p>
      My current research focus is applying machine learning to the sciences. I am currently between
      labs and was recently part of an ongoing research project applying machine learning to optimize
      <a class="link text-blue-400" href="https://en.wikipedia.org/wiki/Stellarator">stellarator</a>
      plasma devices.
    </p>
    <p>
      Previously, I've worked at the
      <a href="https://www.spacecraftresearch.com/" class="link">
        <em>Space Systems Design Studio</em>
      </a>, studying how we can apply computer vision techniques to map lunar materials and regolith
      from stereo images. Before that, I worked on researching quantum algorithms. Specifically, our
      research involved quantifying the effects of decoherence in the variational quantum eigensolver.
    </p>
  </section>
</div>
