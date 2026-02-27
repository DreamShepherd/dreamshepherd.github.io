import Link from "next/link";
import { layout } from "../../components/Layout.js";

Dreaming.getLayout = layout;

const butterfly = 
`      .==-.                   .-==.
       \\()8\`-._  \`.   .'\`  _.-\`8()/
       (88"   ::.  \\./  .::   "88)
        \\_.'\`-::::.(#).::::-'\`._/
          \`._... .q(_)p. ..._.'
            ""-..-'|=|\`-..-""
            .""' .'|=|'. \`"".
          ,':8(o)./|=|\\.(o)8:'.
         (O :8 ::/ \\_/ \\:: 8: O)
          \\O \`::/       \\::' O/
           ""--'         \`--""

`
;

export default function Dreaming({ entries = [] }) {
  const grouped = entries.reduce((acc, entry) => {
    const folder = entry.folder === "." ? "root" : entry.folder;
    if (!acc[folder]) acc[folder] = [];
    acc[folder].push(entry);
    return acc;
  }, {});
  const groups = Object.entries(grouped)
    .map(([folder, notes]) => ({
      folder,
      notes,
    }))
    .sort((a, b) => b.folder.localeCompare(a.folder));
  const isBlogFolder = (folder) => /^\d{4}-\d{2}$/.test(folder);
  const blogGroups = groups.filter((group) => isBlogFolder(group.folder));
  const otherTopLevelGroups = entries
    .filter((entry) => !isBlogFolder(entry.folder))
    .reduce((acc, entry) => {
      const topLevel = entry.folder === "root" ? "root" : entry.folder.split("/")[0];
      if (!acc[topLevel]) acc[topLevel] = [];
      acc[topLevel].push(entry);
      return acc;
    }, {});
  const sortedOtherTopLevelGroups = Object.entries(otherTopLevelGroups)
    .map(([folder, notes]) => ({
      folder,
      notes: notes.sort((a, b) => a.slug.localeCompare(b.slug)),
    }))
    .sort((a, b) => a.folder.localeCompare(b.folder));

  return (
    <div>
      <section className="section font-light">
        <div className="flex justify-center">
          <pre className="inline-block text-left text-xs leading-tight font-mono whitespace-pre text-neutral-700">
            {butterfly}
          </pre>
        </div>
        <p>The musings of a clever fox, and yet it's mostly bleh blah bluh...  
        <br/>Still, I try my best to maximize the ratio of <b><i>|profoundness : word count|</i></b><br/></p>
        <it className="hover:blur-none blur-[2px] text-gray-400 text-xs transition-all">
        <Link className="link" href={`https://biowpn.github.io/bioweapon/`}>psst, if you like cpp, check meowt!</Link>
        </it>

        <div className="pt-4">
          {entries.length === 0 ? (
            <p className="text-sm text-neutral-500">
              No notes yet. Add markdown files to `src/content/dreaming`.
            </p>
          ) : (
            <>
              {blogGroups.map((group) => (
                <div key={group.folder} className="mb-6">
                  <h2 className="font-medium text-black mb-2">
                    {group.folder}
                  </h2>
                  <ul className="space-y-2">
                    {group.notes.map((entry) => (
                      <li key={entry.slug}>
                        <Link className="link" href={`/dreaming/${entry.slug}`}>
                          {entry.title}
                        </Link>
                        {entry.kind === "pdf" ? (
                          <span className="text-xs text-neutral-500"> (pdf)</span>
                        ) : null}
                      </li>
                    ))}
                  </ul>
                </div>
              ))}
              <div className="pt-6 flex justify-center">
                <img
                  src="/static/vulnerable.png"
                  alt="vulnerable"
                  className="w-72 sm:w-80 md:w-96 h-auto"
                />
              </div>
              <br/>
              <hr className="mb-6" />
              {sortedOtherTopLevelGroups.length > 0 ? (
                <div className="mb-6">
                  <h2 className="font-medium text-black mb-3">others</h2>
                  {sortedOtherTopLevelGroups.map((group) => (
                    <div key={group.folder} className="mb-4">
                      <details>
                        <summary className="text-sm text-neutral-600 mb-1 cursor-pointer">
                          {group.folder}
                        </summary>
                        <ul className="space-y-2">
                          {group.notes.map((entry) => (
                            <li key={entry.slug}>
                              <Link className="link" href={`/dreaming/${entry.slug}`}>
                                {entry.title}
                              </Link>
                              {entry.kind === "pdf" ? (
                                <span className="text-xs text-neutral-500"> (pdf)</span>
                              ) : null}
                            </li>
                          ))}
                        </ul>
                      </details>
                    </div>
                  ))}
                </div>
              ) : null}
            </>
          )}
        </div>
      </section>
    </div>
  );
}

export function getStaticProps() {
  const { getDreamEntries } = require("../../lib/dreaming");
  return {
    props: {
      entries: getDreamEntries(),
    },
  };
}
