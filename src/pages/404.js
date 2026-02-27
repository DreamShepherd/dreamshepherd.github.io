import { layout } from "../components/Layout.js";

ErrorPage.getLayout = layout;

export default function ErrorPage() {
  return (
    <div className="layout pt-12">
      <section className="space-y-4">
        <img
          src="/static/doge.gif"
          alt="doge gif"
          className="max-w-full h-auto mx-auto"
        />
        <p className="text-center">how unexpected...</p>
      </section>
    </div>
  );
}
