import { layout } from "../../components/Layout.js";
Remarks.getLayout = layout;

export default function Remarks() {
  return (
    <div>
      <section className="section font-light">
        <p className="font-normal">
          <em>
          This semi-secret page hosts links, errata, and additional information to save space in no particular order as needed in the main site. Will probably remain sparse until I come back to update it.
          </em>
        </p>
        <div className="py-4">
        <li id = "emt">
          Search <a href = "https://www.nremt.org/verify-credentials" className="link">here</a> for id 8652-2556-4757.
        </li>
        </div>
      </section>
    </div>
  );
}
