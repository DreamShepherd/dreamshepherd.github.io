import { layout } from "../../components/Layout.js";
Random.getLayout = layout;

export default function Random() {
  return (
    <div>
      <section className="section font-light">
        <p className="font-normal">
          <em>
          Dump of links and additional information to save space.
          </em>
        </p>
        <div className="py-4">
        <section id = "emt">
          Search <a href = "https://www.nremt.org/verify-credentials" className="link">here</a> for id 8652-2556-4757.
        </section>
        </div>
      </section>
    </div>
  );
}
