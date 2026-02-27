import React from "react";
import { ArrowUpRight } from "lucide-react";

export default function LinkList({ data }) {
  return (
    <div className="grid gap-y-4">
      {data.map((item, index) => (
        <a
          id={item.title}
          key={index}
          href={item.link}
          className="block -mx-3 px-3 py-2 hover:bg-neutral-100 transition-colors"
          target="_blank"
          rel="noreferrer"
        >
          <div className="flex flex-col sm:flex-row sm:items-end mb-1.5">
            <div className="text-lg text-black font-normal">
              {item.title}&nbsp;
              <ArrowUpRight size={18} className="inline text-neutral-400" />
            </div>
            <div className="sm:ml-auto mb-0.5 text-neutral-500">
              {item.date}
            </div>
          </div>
          <div className="text-lg leading-snug font-serif italic">
            {item.summary}
          </div>
        </a>
      ))}
    </div>
  );
}
