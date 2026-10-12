<script>
  import NoLinkList from '#lib/components/NoLinkList.svelte';
  import data from '#lib/data/resume.json';
</script>

<div>
  <section class="section font-light">
    <h1 class="heading"><span class="text-teal-600">#</span> Education</h1>

    <div class="mb-4">
      <div class="flex justify-between items-end">
        <h2 class="text-black text-lg font-normal">Cornell University</h2>
        <div class="text-neutral-500 font-normal">2021 — 2024</div>
      </div>
      <p class="font-serif italic">B.S. Computer Science, EP</p>
    </div>
    <p class="mb-4">
      Binu Thomas Scholar, Summa Cum Laude (top 1% of college of engineering), Award for Excellence
      in Physics, with highest honors in field
    </p>

    <div>
      <h1 class="font-medium mt-4">Teaching</h1>
      <ul class="space-y-1">
        <li>- TA for CS 5756 (Reinforcement Learning in Robotics)</li>
        <li>- TA for CS 2110 (Data Structures and Algorithms)</li>
        <li>
          - Advised for
          <a class="link" href="https://en.wikipedia.org/wiki/Governor%27s_School_of_New_Jersey">
            Governor's School in the Sciences (NJGSS)
          </a> as a former alum, Summer 2022
        </li>
      </ul>
    </div>
    <div>
      <p class="font-medium">Technical Coursework</p>
      <a class="link" href="#coursework">Graduate-level and undergraduate-level technical coursework</a>
    </div>
  </section>

  <section class="section font-light">
    <h1 class="heading"><span class="text-teal-600">#</span> Programming</h1>
    <p>
      I'm interested in <span class="font-semibold">statistical analysis</span>,
      <span class="font-semibold">machine learning</span>, and
      <span class="font-semibold">user design</span>. Check out my
      <a class="link text-blue-400" href="https://github.com/dreamShepherd">github</a>. I use the
      following tools:
    </p>
    <p class="leading-relaxed text-sm">
      <span class="font-semibold">Languages:</span> Python · Java · C/C++ · HTML/CSS · Javascript ·
      Wolfram Language · LaTeX <br />
      <span class="font-semibold">Frameworks:</span> NextJS · ReactJS · Tailwind · Yaml · ROS ·
      Tensorflow/Keras · PyTorch
      <br />
      <span class="font-semibold">Tools/Services:</span> VSCode · MongoDB · Heroku · Git/Github ·
      Vowpal Wabbit
    </p>
  </section>
  <br />
  <a id="coursework">
    <div class="bg-gray-900 py-12 text-neutral-200">
      <section class="layout">
        <h1 class="heading text-white">Technical Coursework</h1>
      </section>
    </div>
  </a>

  <section class="layout pt-12">
    <h1 class="heading">Computer Science</h1>
    <NoLinkList data={data.coursework} />
    <h1 class="heading pt-12">Math and Physics</h1>
    <NoLinkList data={data.physcoursework} />
  </section>
</div>
