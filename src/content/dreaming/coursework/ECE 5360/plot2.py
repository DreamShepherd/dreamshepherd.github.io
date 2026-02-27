import matplotlib.pyplot as plt

plt.figure(figsize=(8, 6))

print("Type: (CFB)")
typ = input()

for i in ['1e16', '1e17', '1e18', '1e19']:
    with open(f"schottky_out{i}.txt", "r") as file:
        lines = file.readlines()

    header = lines[0].strip().split("\t")
    rows = [line.strip().split("\t") for line in lines[1:]]

    parsed_data = [dict(zip(header, row)) for row in rows]

    # Y (ang)	Ec (eV)	Ev (eV)	E (V/cm)	Ef (eV)	n (cm-3)	p (cm-3)	Nd - Na (cm-3)

    y_ang = [float(entry["Y (ang)"]) for entry in parsed_data]
    ec_ev = [float(entry["Ec (eV)"]) for entry in parsed_data]
    ev_ev = [float(entry["Ev (eV)"]) for entry in parsed_data]
    e_vcm = [float(entry["E (V/cm)"]) for entry in parsed_data]
    ef_ev = [float(entry["Ef (eV)"]) for entry in parsed_data]
    n_cm3 = [float(entry["n (cm-3)"]) for entry in parsed_data]
    p_cm3 = [float(entry["p (cm-3)"]) for entry in parsed_data]
    nd_na_cm3 = [float(entry["Nd - Na (cm-3)"]) for entry in parsed_data]



    match typ:
        case "C":
            # CHARGES
            plt.plot(y_ang, n_cm3, linestyle="-", label=f"n (cm-3) {i}")
            plt.plot(y_ang, p_cm3, linestyle="-", label=f"p (cm-3) {i}")
            plt.plot(y_ang, nd_na_cm3, linestyle="-", label=f"Nd - Na (cm-3) {i}")

        case "F":
            # FIELD
            plt.plot(y_ang, e_vcm, linestyle="-", label=f"E (V/cm) {i}")
        case "B":
            # BAND
            plt.plot(y_ang, ec_ev, linestyle="-", label=f"Ec (eV) {i}")
            plt.plot(y_ang, ev_ev, linestyle="-", label=f"Ev (eV) {i}")
            plt.plot(y_ang, ef_ev, linestyle="-", label=f"Ef (eV) {i}")

plt.xlabel("Y (ang)")
plt.legend()
plt.grid(True)
plt.show()
