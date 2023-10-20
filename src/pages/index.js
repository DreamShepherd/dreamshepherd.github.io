import React from "react";
import { layout } from "../components/Layout.js";
import QuickLinks from "../components/QuickLinks.js";
About.getLayout = layout;

export default function About() {
  return (
    <div className="layout">
      <p className="text-black text-xl leading-tight font-light mb-8">
        <span className="gem">is a </span> software engineer
        <span className="gem">,</span>
        <br />
        physicist<span className="gem">, and</span>
        <br />
        researcher<span className="gem">.</span>
      </p>
      <QuickLinks />

      <div className="space-y-8">
        <section className="space-y-4 text-md">
          <p>
            Hello! I currently study{" "}
            <span className="text-blue-500">engineering physics</span> and{" "}
            <span className="text-blue-500">computer science</span> and{" "}
            <span className="text-blue-500">electrical engineering</span> at{" "}
            <b>Cornell University</b>. My academic interests involve
            reinforcement learning and robotics. I am currently working at Snowflake for the spring in software engineering.
          </p>
          <p>
            My professional experience involves machine learning and statistical
            analysis. I also enjoy chess, tennis, working out, playing piano,
            memorizing poetry, and listening to music.
          </p>
        </section>
        <section>
        <h1 className="heading ">
          <span className="text-teal-600">#</span> Some (Possibly?) Fun Facts {" "}
        </h1>
        <ul className="text-sm grid gap-1  grid-cols-2 text-neutral-700">
          <li>- Memorized 400 digits of pi once</li>
          <li>- Was an EMT during COVID{" "}
             <a className = 'link' href= "/remarks#emt">#646230</a></li>
            
          <li>- 1590/1600 SAT first try while sick</li>
          <li>- Rated 1800 in blitz chess; 2500 puzzles</li>
          <li>- 16.1s PB solving the Rubiks Cube</li>
          <li>- Used to solve cubes blindfolded</li>
          <li>- Can recite six poems</li>
          <li>- Hit masters in Starcraft II</li>
          <li>- Hit legend in Hearthstone</li>
          <li>- 1:34 PB in <a className = 'link' href= 'https://www.setwithfriends.com'>SET</a></li>
          <li>- Enjoys tennis, rock climbing, and pool</li>
          <li>- Verilog sucks...</li>
        </ul>
      </section>
        <section className="text-sm space-y-4">
          <h1 className="heading">
            <span className="text-teal-600">#</span> Programming
          </h1>
          <p>
            I'm interested in{" "}
            <span className="font-semibold">web development</span>,{" "}
            <span className="font-semibold">robotics</span>, and{" "}
            <span className="font-semibold">user design</span>. Check out my{" "}
            <a
              className="link text-blue-400"
              href="https://github.com/dreamShepherd"
            >
              github
            </a>
            . I use the following tools:
          </p>
          <p className="leading-relaxed text-sm">
            <span className="font-semibold">Languages:</span> Python · Java ·
            C/C++ · HTML/CSS · Javascript · Wolfram Language · LaTeX <br />
            <span className="font-semibold">Frameworks:</span> NextJS · ReactJS
            · Tailwind · Yaml · ROS · Tensorflow/Keras · PyTorch
            <br />
            <span className="font-semibold">Tools/Services:</span> VSCode ·
            MongoDB · Heroku · Git/Github ·{" "}
            <a href="https://vowpalwabbit.org/" className="link">
              Vowpal Wabbit
            </a>
          </p>
        </section>
        <section className="text-sm space-y-4">
          <h1 className="heading">
            {" "}
            <span className="text-teal-600">#</span> Research
          </h1>
          <p>
            My current research focus is applying machine learning to the
            sciences. I'm currently in between labs. 
            {/* part of an ongoing research project applying machine
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
            eigensolver.  */}
          </p>
        </section >
      </div>
    </div>
  );
}
