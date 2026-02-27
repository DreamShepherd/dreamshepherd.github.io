import Header from "./Header";
import React from "react";
import Link from "next/link";
import { AnimatePresence, motion } from "framer-motion";
import { ArrowUpToLine, Copyright } from "lucide-react";

const layout = (page, key) => (
  <>
    <div
      className={key.startsWith("/dreaming") ? "pt-6 pb-12 sm:pb-24 md:pb-32" : "margin"}
      id="top"
    >
      {key.startsWith("/dreaming") ? (
        <div className="layout sticky top-0 z-20 bg-white/90 backdrop-blur-sm pb-4">
          <h1 className="text-black text-2xl md:text-3xl font-semibold my-1 pt-2">
            <Link scroll={false} href="/dreaming">
              Dreaming ↔ Butterflies
            </Link>
          </h1>
          <p
            className="whitespace-nowrap overflow-hidden text-xl leading-none opacity-40 select-none pt-2"
            aria-hidden="true"
          >
            oh, 🐫, oh 🐫, oh, 🐫, oh 🐫, oh, 🐫, oh 🐫, oh, 🐫, oh 🐫, oh, 🐫, oh 🐫
          </p>
        </div>
      ) : (
        <Header />
      )}
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
      <a href = '#top'><ArrowUpToLine size = {24}/></a>
    </div>
    <div className="text-xs opacity-50 pb-2 font-sans">
      <div className="flex place-content-center space-x-0.5 items-center">
        <p className="text-xs"> Bryant Har</p>
      </div>
    </div>
  </>
);

export { layout };
