import matplotlib.pyplot as plt

with open("grading_linear_out.txt", "r") as file:
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


fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(6, 10), sharex=True)



ax1.set_title("Charge")
ax1.set_ylabel("Charge (C/cm^3)")

ax2.set_title("Field")
ax2.set_ylabel("Field (V/cm)")

ax3.set_title("Band")
ax3.set_ylabel("Energy (eV)")

ax1.plot(y_ang, [nd_na_cm3[i] - n_cm3[i]  if nd_na_cm3[i] > 0 else p_cm3[i] + nd_na_cm3[i]    for i in range(len(y_ang))], linestyle="-", label="Nd - Na (cm-3)", color="red")
ax2.plot(y_ang, e_vcm, linestyle="-", label="E (V/cm)", color="green")
ax3.plot(y_ang, ec_ev, linestyle="-", label="Ec (eV)")
ax3.plot(y_ang, ev_ev, linestyle="-", label="Ev (eV)")
ax3.plot(y_ang, ef_ev, linestyle="-", label="Ef (eV)")

plt.xlabel("x (ang)")
plt.legend()
plt.grid(True)
plt.show()
