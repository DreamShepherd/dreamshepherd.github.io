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
            <span className="text-blue-500">physics</span> and{" "}
            <span className="text-blue-500">computer science</span> and{" "}
            <span className="text-blue-500">English</span> at{" "}
            <b>Cornell University</b>. My academic interests involve
            reinforcement learning and robotics, though I try to be a
            generalist.
          </p>
          <p>
            My professional experience involves machine learning and statistical
            analysis. I also enjoy chess, tennis, working out, playing piano,
            memorizing poetry, and listening to music.
          </p>
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
        <section className=" text-sm space-y-4">
          <h1 className="heading">
            {" "}
            <span className="text-teal-600">#</span> Research
          </h1>
          <p>
            My current research focus is applying machine learning to the
            sciences. I'm part of an ongoing research project applying machine
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
            quantifying the effects of decoherence in the variational quantum
            eigensolver.
          </p>
        </section>
      </div>
    </div>
  );
}
