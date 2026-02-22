import defaultTheme from "tailwindcss/defaultTheme"

/** @type {import("tailwindcss").Config} */
const config = {
  content: ["./src/**/*.{html,js,svelte,ts,tsx}"],

  darkMode: "class",

  theme: {
    extend: {
      fontFamily: {
        sans: [...defaultTheme.fontFamily.sans],
        serif: ['Newsreader', ...defaultTheme.fontFamily.serif],
      },
    },
  },

  plugins: [],
};

module.exports = config;