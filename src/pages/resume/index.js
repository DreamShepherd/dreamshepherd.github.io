import data from "./resume.yaml";
import NoLinkList from "../../components/NoLinkList";
import { layout } from "../../components/Layout.js";
Resume.getLayout = layout;

export default function Resume() {
  return (
    <div>
      <section className="section font-light">
        <h1 className="heading">
          <span className="text-teal-600">#</span> Education
        </h1>

        <div className="mb-4">
          <div className="flex justify-between items-end">
            <h2 className="text-black text-lg font-normal">
              Cornell University
            </h2>
            <div className="text-neutral-500 font-normal">2021 — 2025</div>
          </div>
          <p className="font-serif italic">B.S. Computer Science</p>
        </div>
        <p className="font-small mb-4">
          Binu Thomas Scholar, Summa Cum Laude (top 1% of college), Award for Excellence in Physics, 
          with highest honors in field
        </p>

        <div>
          <h1 className="font-medium mt-4">Teaching</h1>
          <ul className="space-y-1">
            <li>
              - TA for CS 2110 (Data Structures and Algorithms)
              - TA for CS 4/5756 (Data Structures and Algorithms)
            </li>
            <li>
              - Advised for{" "}
              <a
                className="link"
                href="https://en.wikipedia.org/wiki/Governor%27s_School_of_New_Jersey"
              >
                Governor's School in the Sciences (NJGSS)
              </a>
              , Summer 2022
            </li>
          </ul>
        </div>
        <div>
          <p className="font-medium">Technical Coursework</p>
          <a className="link" href="#coursework">
            Graduate-level and undergraduate-level technical coursework
          </a>
        </div>
      </section>

      <section className="section font-light">
        <h1 className="heading">
          <span className="text-teal-600">#</span> Programming
        </h1>
        <p>
          I'm interested in{" "}
          <span className="font-semibold">statistical analysis</span>,{" "}
          <span className="font-semibold">machine learning</span>, and{" "}
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
          <span className="font-semibold">Frameworks:</span> NextJS · ReactJS ·
          Tailwind · Yaml · ROS · Tensorflow/Keras · PyTorch
          <br />
          <span className="font-semibold">Tools/Services:</span> VSCode ·
          MongoDB · Heroku · Git/Github ·{" "}
          <a href="https://vowpalwabbit.org/" className="link">
            Vowpal Wabbit
          </a>
        </p>
      </section>

      <section className="section pb-12">
        <h1 className="heading ">
          <span className="text-teal-600">#</span> Fun Facts{" "}
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

      <a id="coursework">
        <div className="bg-gray-900 py-12 text-neutral-200 dark">
          <section className="layout">
            <h1 className="heading text-white">Technical Coursework</h1>
          </section>
        </div>
      </a>

      <section className="layout pt-12">
        <h1 className="heading"> Computer Science </h1>
        <NoLinkList data={data.coursework}> </NoLinkList>
        <h1 className="heading pt-12"> Math and Physics </h1>
        <NoLinkList data={data.physcoursework}> </NoLinkList>
      </section>
    </div>
  );
}
