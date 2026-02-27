import numpy as np

from numpy import log, exp

epsilon_vacuum= 8.8541878188*10**(-14) # F/cm
kt= 0.026
q=1.602*10**(-19)
m=9.11*10**(-31)

Eg = 1.124
epsilon=11.7*epsilon_vacuum # sil 
epsilon_gaas=13.1*epsilon_vacuum # gaas
nc_sil = 2.8e19
nc_gaas = 4.7e17
nv_sil = 6e18
ni = 1.45* 10**10

nm=10**(-7) #cm
um =10**(-4) #cm
fm =10**(-13) #cm
pi = 3.14159265359

dn_sil = 20
dp_sil = 10

################################################
# Work Station
################################################

dp = 10
dn = 228
niA = 2.1e6
na = 1e15
nd = 2e17
taun = 2e-7
taup = 2e-7
ln = (dn * taun) ** (1/2)
lp = (dp * taup) ** (1/2)

res = q * niA * niA * (dp / (nd * lp) + dn / (na * ln))

# ni = 6.68e9
# res = kt * log(1e34/(ni * ni))

# 5.2
tau = 10e-6
na = 1e17
nd = 1e17
lp = (dp_sil * tau) ** (1/2)
ln = (dn_sil * tau) ** (1/2)

res = q * ni * ni * (dp_sil / (nd * lp) + dn_sil / (na * ln))

# 5.14
k = kt / 300
res = 1/ (2 * k * log(1e4 + 1))

A = 1e-5
tau=1e-7
lp = (dp_sil * tau) ** (1/2)
ln = (dn_sil * tau) ** (1/2)
nd = 1e15
na = 5e18
nini = 1e-7 / (A * q * (dp_sil / (nd * lp) + dn_sil / (na * ln)))
res = Eg / (k * log(nv_sil * nc_sil / (nini)))

# 5.18
A = 1e-5
W = 3e-4
na = 1e17
i = 5e-4
expon= i / (A * q * ni * ni * (dn_sil / (na * W)))
va = 0 # log(expon + 1) * kt
phii = 0.56+ kt* log(na / ni)
xp = (2 * epsilon * -(va - phii) / (q * na))**(1/2)
qq = -q * na * (A * xp)
t2 = (-8.74e-13 - qq )/ i
res = 2.24 + 1.84

# 5.2 Current
tau=10e-6
lp = (dp_sil * tau) ** (1/2)
ln = (dn_sil * tau) ** (1/2)
nd = 1e17
na = 1e17
nii = 6.68e9
va = 0.339
expon = exp(va / kt) - 1
res = expon * q * nii * nii * (dp_sil / (nd * lp) + dn_sil / (na * ln))

# 5.3

tau=0.2e-6
dp = 10
dn = 228
lp = (dp * tau) ** (1/2)
ln = (dn * tau) ** (1/2)
nd = 2e17
na = 1e15
nii = 2.1e6

pno = (nii * nii) / nd
npo = (nii * nii) / na



va = 0.5
Jp = q * nii * nii * (dp / (nd * lp))
Jn = q * nii * nii * (dn / (na * ln))
Jo = Jp + Jn
expon = exp(va / kt) - 1

wb = 2 * um
Jp_short = q * nii * nii * (dp / (nd * lp ))
Jn_short = q * nii * nii * (dn / (na * wb))
Jo_short = Jp_short + Jn_short

sigma = 5e11
Qa = sigma * q
nd = nc_gaas
l1 = 100* nm
l2 = 200 * nm

xdr = (Qa**2 + 2*q*nd * Qa * l2) / (2*q * nd * (q * nd * (l1 + l2) + Qa))
xd2 = (Qa**2 + 2*q*nd * Qa * l1) / (2*q * nd * (q * nd * (l1 + l2) + Qa))
Vr = 1/2 * (nd / epsilon_gaas) * xdr **2  + l1 * xdr * nd / epsilon_gaas
Vo = 1/2 * (q * nd / epsilon_gaas) * xd2 **2  + l2 * xd2 * nd * q/ epsilon_gaas


xd1 = l2 # (Qa / (nd * q))
forw = q * ((xd1 * xd1 * nd / (2 * epsilon_gaas)) + l1* xd1 * nd / epsilon_gaas)

rev = q * ((xd1 * xd1 * nd / (2 * epsilon_gaas)) + l2 * xd1 * nd / epsilon_gaas)


res = l1 * l1 * (3/2) * (nd * q / epsilon_gaas)


lamb = 0.45e-6
c = 3e8
w = 2 * np.pi * c / lamb
mu = 4 * np.pi * 1e-7
ep = 4 * epsilon_vacuum*100

w = 1e9 * 2 * pi
tau = epsilon_vacuum * 100 / 5

res = w * tau

qeo = 8e9 / q
a = 1e-5
dpe=2
dnb = 18
na=4e17
xb=0.5*um
tau = 1e-7



gamma = (1 + q * a * na * xb * dpe / (dnb * qeo)) ** (-1)
gamma = 0.997
att = 1 - (xb * xb) / (2 * dnb * tau)
af = gamma * att
bf  = af / (1-af)
qbo = q * a * na * xb 
bfapprox = qeo * dnb / qbo
res = (336 - 325 )/ 325

N=1e17
sigma=5e11
w = (sigma / N) * (np.sqrt(2) - 1)


Wb = 200 * nm

mu = 8500
Dn = 200
Dp = 10
Eb = 6235



We = 400 * nm
tau = 0.3e-12
tau_bjt=1e-12
NdeBJT=1e18
NabBJT=5e17
gnB_bjt = NabBJT * Wb
gnE_bjt = NdeBJT * We
alphaT_bjt =  (Wb * Wb) / (2 * Dn * tau_bjt)
gamma_BJT = (1 + (gnB_bjt * Dp) / (gnE_bjt * Dn)) ** (-1)


res = gamma_BJT
res = alphaT_bjt
res = Wb * Wb
res = 2 * Dn * tau_bjt
res = alphaT_bjt


mu = 670
L = 0.8*um
v = (3) * mu / (2 * L)
x = 15 * nm
E = 3/(6 * x) + (0.7+0.5)/(3 * x)
muef = mu / (1 + (0.67/0.6) ** 1.6)
vsat = 8e6
muef= 350
esat = vsat * 2/ muef
vdsat = 1.65
res = (3 - vdsat) * vsat / 3

#nmos
vsat = 8e6
mu = 670
VT = 0.7
VZ = 0.5
VG = 5
xox = 15 * nm
L = 0.5 * um
W = 100*um
Eeff = ((VG - VT) / (6 * xox) + (VT+ VZ) / (3 * xox)) / (1e6)
eta  = 1.6
Cox = 3.9 * epsilon_vacuum / xox
E0 = 0.67

mueff = mu / (1 + (Eeff/E0)**(eta))

Esat = 2 * vsat / mueff

VDsat = (Esat * L * (VG - VT)) / (Esat * L + (VG-VT))
Id = (W * Cox * (VG - VT - VDsat) * vsat) * 1000
gmsat = W* vsat * Cox * ((VG-VT) * (VG-VT+2*Esat* L))/((VG-VT+Esat * L)**2)
res = gmsat


#pmos
vsat = 8e6
mu = 160
VT = 0.7
VZ = 0.5
VG = 5
xox = 15 * nm
L = 0.5 * um
W = 100*um
Eeff = ((VG - VT) / (6 * xox) + (VT+ VZ) / (3 * xox)) / (1e6)
eta = 1
Cox = 3.9 * epsilon_vacuum / xox
E0 =0.7

mueff = mu / (1 + (Eeff/E0)**(eta))

Esat = 2 * vsat / mueff

VDsat = (Esat * L * (VG - VT)) / (Esat * L + (VG-VT))
Id = (W * Cox * (VG - VT - VDsat) * vsat) * 1000

gmsat = W* vsat * Cox * ((VG-VT) * (VG-VT+2*Esat* L))/((VG-VT+Esat * L)**2)
res = gmsat




nd = 2e17
xn = np.sqrt((2 * epsilon) * (Eg - kt * np.log(nd/ni)) / (q * nd))
na = nv_sil
phii = kt * np.log(nd * na / (ni * ni))

xnpp = np.sqrt((2 * epsilon * phii) * (1/na + 1/nd) / (q))

xnp = xnpp / (1 + nd/na)

xo = (xnp + xn + 20 * nm )

res = phii - (q * nd * xo * xo / (2 * epsilon))

I = 0.01
A = 1e-5
N=1e22
v=1e7

theta = I / (A * N * v * q)
eg = 1.1
B = 7.87e7
B = np.sqrt(0.25) * B
L = np.log(1/(theta)) * eg / (B)
e = eg/(L)
res = e


epsilonox = 3.9 * epsilon_vacuum 
d1 = 10 * nm
qfg = (epsilonox / d1)

vsat = 8e6
mu = 670
VT = 1.7
VZ = 0.5
VG = 6
xox = 10 * nm
L = 0.5 * um
W = 100*um
Eeff = ((VG - VT) / (6 * xox) + (VT+ VZ) / (3 * xox)) / (1e6)
eta  = 1.6
Cox = 3.9 * epsilon_vacuum / xox
E0 = 0.67

mueff = mu / (1 + (Eeff/E0)**(eta))

Esat = 2 * vsat / mueff

VDsat = (Esat * L * (VG - VT)) / (Esat * L + (VG-VT))
Id = (W * Cox * (VG - VT - VDsat) * vsat) 

chargeInjected = (qfg * 0.5 * um * um * 100)

subres = 1 * um * Cox * (6 - 1.7 - 2.05) * vsat * 1000
res = VDsat


# quiz 6

phib = 1
#  epsilon = 10 * epsilon_vacuum
piezodens = 1.5e13
mu=1500
tc=2 * nm
tb = epsilon * (phib + 4) / (piezodens * q)
Vt = phib - (tb * piezodens * q) / (epsilon)
vg = 0.5
cc = epsilon / tc
cb = epsilon / tb
cq = 386 * (10**(-14)) / um
cg = 1/(1/cc + 1/cq + 1/cb)
W = 100 * um
L = mu * W * cg * ((vg - Vt) ** 2) / (2 )

res = L / nm


#HW 11

phib = 4.5 * q 
na = 2e17
tox = 10 * nm
epsilon_sil = 12 * epsilon_vacuum
epsilon_ox = 4 * epsilon_vacuum
cox = epsilon_ox / tox

vt = 2 * phib + np.sqrt(2 * epsilon * q * na * 2 * phib) / cox
res = vt





###############################################
print()
print("##### RESULT #####")
print("{:.10e}".format(res))
print()


