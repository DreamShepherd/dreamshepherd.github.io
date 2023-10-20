import Header from "./Header";
import React from "react";
import { AnimatePresence, motion } from "framer-motion";
import { ArrowUpToLine, Copyright } from "lucide-react";

const layout = (page, key) => (
  <>
    <div className="margin" id = 'top'>
      <Header />
      <AnimatePresence mode="wait">
        <motion.div
          initial={{ x: -10, opacity: 0 }}
          animate={{ x: 0, opacity: 1, ease: "linear" }}
          transition={{ duration: 0.25, delay: 0 }}
          exit={{ y: 5, opacity: 0 }}
          key={key}
        >
          {page}
        </motion.div>
      </AnimatePresence>
      <section className="layout">
        <ul className="text-sm font-light mt-8 space-y-1.5">
          <li>
            Email:{" "}
            <a className="link" href="mailto:bhar@bryanthar.com">
              bhar@bryanthar.com
            </a>
          </li>
          <li>
            Github:{" "}
            <a className="link" href="https://github.com/dreamShepherd">
              @DreamShepherd
            </a>
          </li>
          <li>
            Linkedin:{" "}
            <a
              className="link"
              href="https://www.linkedin.com/in/bryant-har/"
            >
              @bryant-har
            </a>
          </li>
        </ul>
      </section>
    </div>
    
    <div className="flex justify-end pb-8 pr-16">
      <a href = '#top'><ArrowUpToLine size = {36}/></a>
    </div>
    <div className="text-xs opacity-50 pb-2 font-sans">
      <p className="flex place-content-center space-x-0.5">
        <a className="link no-underline" href="https://github.com/dreamShepherd/bryanthar.com">
          Source Code
        </a>{" "}&nbsp;| Powered by NextJS
      </p>
      <div className="flex place-content-center space-x-0.5 items-center">
        <Copyright size={16} />
        <p className="text-xs"> 2022 - 2023 Bryant Har | <a href='https://creativecommons.org/licenses/by-nc/4.0/' className="link no-underline">CC BY-NC 4.0</a></p>
      </div>
    </div>
  </>
);

export { layout };
