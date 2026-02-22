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
            Hello! FYI, this website is only serviced when I'm bored. I am a BS {" "}
            <span className="text-blue-500">engineering physics</span> and{" "}
            <span className="text-blue-500">computer science</span> and{" "}
            <b>Cornell University</b>. My academic interests involve
            topology, reinforcement learning, various physics topics, functional programming, and robotics.
          </p>
          <p>
            I also enjoy chess, tennis, working out, playing piano,
            memorizing poetry, and listening to music.
          </p>
        </section>
        <section className="text-sm space-y-4">
          <h1 className="heading">
            <span className="text-teal-600">#</span> Interests
          </h1>
          <p>
            I'm interested in{" "}
            <span className="font-semibold">puzzles</span>,{" "}
            <span className="font-semibold">hardware</span>, and{" "}
            <span className="font-semibold">user design</span>. Check meowt! I use the following tools:
          </p>
          <p className="leading-relaxed text-sm">
            <span className="font-semibold">Languages:</span> Python · Java ·
            C/C++ · HTML/CSS · Javascript · Wolfram Language · LaTeX <br />
            <span className="font-semibold">Frameworks:</span> NextJS · ReactJS
            · Tailwind · Yaml · ROS · Tensorflow/Keras · PyTorch
            <br />
            <span className="font-semibold">Tools/Services:</span> VSCode ·
            MongoDB · Heroku · Git/Github · Vowpal Wabbit
          </p>
        </section>
        <section>
          <h1 className="heading ">
            <span className="text-teal-600">#</span> Fun Facts {" "}
          </h1>
          <ul className="text-sm grid gap-1  grid-cols-2 text-neutral-700">
            <li>- Pretty big Lewis Carroll fan</li>
            <li>- Pretty big puzzle fan!</li>
            <li>- Was an EMT during COVID <a className = 'link' href= "/random#emt">*</a></li>
            <li>- 1590/1600 SAT first try (sick with covid lol)</li>
            <li>- Have hit 1800 elo in chess</li>
            <li>- 16.1s PB solving the Rubiks Cube</li>
            <li>- Used to solve cubes blindfolded</li>
            <li>- Can recite six poems</li>
            <li>- Recited 400 digits of pi in middle school</li>
            <li>- I do the monthly Jane Street puzzle if not bashy.<a className = 'link' href= "/random#emt">*</a></li>
            <li>- Hit masters in Starcraft II<a className = 'link' href= "/random#emt">*</a></li>
            <li>- Hit legend in Hearthstone</li>
            <li>- 1:24 PB in <a className = 'link' href= 'https://www.setwithfriends.com'>SET</a></li>
            <li>- Enjoys tennis, rock climbing, and pool</li>
            <li>- I actually think Verilog is neat...</li>
            <li>- Took a Spanish literary analysis course in Spanish<a className = 'link' href= 'https://www.setwithfriends.com'>*</a></li>
            {/* read don quixote and spanish poetry in spanish)...*/}
          </ul>
        </section>
        {/*
        <section className="text-sm space-y-4">
          <h1 className="heading">
            {" "}
            <span className="text-teal-600">#</span> Research
          </h1>
          <p>
            My current research focus is applying machine learning to the
            sciences. I'm currently in between labs. 
            part of an ongoing research project applying machine
            learning to optimize{" "}
            <a
              className="link text-blue-400"
              href="https://en.wikipedia.org/wiki/Stellarator"
            >
              stellarator
            </a>{" "}
            plasma devices.
          </p>
          <p>
            Previously, I've worked at the{" "}
            <a href="https://www.spacecraftresearch.com/" className="link">
              <em>Space Systems Design Studio</em>
            </a>
            , studying how we can apply computer vision techniques to map lunar
            materials and regolith from stereo images. Before that, I worked on
            researching quantum algorithms. Specifically, our research involved quantifying the effects of decoherence in the variational quantum
            eigensolver.  
          </p>
        </section >
        */}
      </div>
    </div>
  );
}
