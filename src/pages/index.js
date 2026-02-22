import React from "react";
import { layout } from "../components/Layout.js";
import QuickLinks from "../components/QuickLinks.js";
About.getLayout = layout;

export default function About() {
  return (
<div className="layout">
  <p className="text-black text-xl leading-tight font-light mb-8">
    is thinking hard <br />
    and dreaming of <br />
    butterflies <br />
  </p>
  <QuickLinks />

  <div className="space-y-8">
    <section className="space-y-4 text-md">
      <p>
        I try to maximize my profoundness/wordcount ratio <a className="link" href="www.google.com">here</a>.
      </p>
      <p>
        Hello! FYI, this website is only serviced when I'm bored. I have a B.S. {" "}
        <span className="text-blue-500">engineering physics</span> and 
        <span className="text-blue-500"> computer science</span> from
        <b> Cornell University</b>. My academic interests involve
        topology, reinforcement learning, various physics topics, functional programming, and robotics.
      </p>
      <p>
        I'm also a casual enjoyer chess, tennis, working out, running, playing piano,
        Ancient civilizations, poetry, and listening to music.
      </p>
    </section>
    <section className="text-sm space-y-4">
      <h1 className="heading"><span className="text-teal-600">#</span> Interests</h1>
      <p>
        I'm interested in{" "}
        <span className="font-semibold">puzzles</span>,{" "}
        <span className="font-semibold">hardware</span>, and{" "}
        <span className="font-semibold">user design</span>. Check meowt!
      </p>
      <p className="leading-relaxed text-sm">
        <span className="font-semibold">Languages:</span> Python · Java ·
        C/C++ · OCaml · Javascript · Wolfram Language · LaTeX <br />
        <span className="font-semibold">Frameworks:</span> NextJS · ReactJS
        · Tailwind · Yaml · ROS · Tensorflow/Keras · PyTorch
        <br />
        <span className="font-semibold">Tools/Services:</span> VSCode ·
        MongoDB · Heroku · Git/Github · Vowpal Wabbit
      </p>
    </section>
    <section>
      <h1 className="heading "><span className="text-teal-600">#</span> Fun Facts {" "}</h1>
      <ul className="text-sm grid gap-1  grid-cols-2 text-neutral-700">
        <li>- Pretty big puzzle fan!</li>
        <li>- New Jersey xd</li>
        <li>- I like rhythm games like osu and vocaloid songs.</li>
        <li>- Lewis Carroll fan. I love red queens race references.</li>
        <li>- Was an EMT during COVID <a className = 'link' href= "/random#emt">*</a></li>
        <li>- 1590/1600 SAT first try (sick with covid lol)</li>
        <li>- Have hit 1800 elo in chess</li>
        <li>- 16.1s PB solving the 3x3 Rubiks Cube</li>
        <li>- Formerly could solve cubes blindfolded</li>
        <li>- Can recite six poems</li>
        <li>- Recited 400 digits of pi in middle school</li>
        <li>- I do the monthly Jane Street puzzle if not bashy.<a className = 'link' href= "/random#jspuzzle">*</a></li>
        <li>- Hit masters in Starcraft II<a className = 'link' href= "/random#emt">*</a></li>
        <li>- Hit legend in Hearthstone</li>
        <li>- Hit a 1:27 in <a className = 'link' href= 'https://www.setwithfriends.com'>SET</a></li>
        <li>- I actually think Verilog is pretty neat...</li>
        <li>- Took Spanish lit in Spanish<a className = 'link' href= 'https://www.setwithfriends.com'>*</a></li>
      </ul>
    </section>
  </div>
</div>
  );
}
