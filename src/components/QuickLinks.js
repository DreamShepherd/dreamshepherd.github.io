import { Mail } from "lucide-react";
import Image from 'next/image'
import Typewriter from "typewriter-effect";
import doge from '../../public/static/ico.png'
import git from '../../public/static/github.svg'
import disc from '../../public/static/discord.svg'
import linkedin from '../../public/static/linkedin.svg'
import chess from '../../public/static/lichess.svg'


export default function QuickLinks() {
  return (
    <div className="pb-8">
      <Image
        className="mx-auto rounded-full"
        height="auto"
        width="100"
        src={doge}
      />
      <div className="heading text-2xl pl-1.5 text-center mb-2 text-slate-500">
        <Typewriter
          options={{
            delay:75,
          }}
        

        
          onInit={(typewriter) => {
            typewriter.typeString(" Quick Links").start();
          }}
        />
      </div>
      <div className="flex items-center justify-center space-x-1">
        <a href="https://lichess.org/@/DreamShepherd">
          <Image width="27" height="auto" src={chess}/>{" "}
        </a>
        <a href="https://www.linkedin.com/in/bryant-har/">
          <Image width="23" height="auto" src={linkedin}/>
        </a>
        <a href="https://github.com/DreamShepherd">
          <Image width="24" height="auto" src={git}/>
        </a>
        <a href="https://discordapp.com/users/_thoge">
          <Image width="25" height="auto" src={disc}/>
        </a>
        <a href="mailto:bhar@bryanthar.com">
          <Mail size = {26}/>
        </a>
      </div>
    </div>
  );
}
