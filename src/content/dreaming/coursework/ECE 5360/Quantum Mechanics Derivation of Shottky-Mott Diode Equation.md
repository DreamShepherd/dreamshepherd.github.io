
### a) Derivation
$J=nqv=\frac{qg_sg_v}{L^3}\sum_kf(k)v_g(k)$
Assume distribution of energy quanta is parabolic and isotropic. First, we find $v_g(k)$.
$$E(k,z=0) = E_c(z=0) + \frac{\hbar^2}{2m_c^*}(k_x^2+k_y^2+k_z^2)$$
We define xyz in typical convention starting at the top of the top of $E_c$ curve in the band diagram. $\hat z$ points to the metal and crosses between the semiconductor and the metal. So $E_c(z=0)$ is the top of the $E_c$ curve. Consider a sphere at this point (sphere centered at top of $E_c$ curve in band diagram)
Many states. $E_f$ is the sphere of this. Spacing between states is $2\pi/L_x$ or $2\pi/L_y$ or $2\pi/L_z$

We only analyze half of this sphere. Left half of states flows in the metal direction. Right half of states flows in the semiconductor direction in spherical coordinates. We look at semiconductor to metal (left half)
$$v_g = \frac{1}{\hbar}\nabla_k E(k)=\frac{\hbar}{m_c^*}(k_x\hat x+k_y \hat y+k_z \hat z)$$
By symmetry, only net velocity in z direction.
$$v_g = \frac{1}{\hbar}\nabla_k E(k)=\frac{\hbar k_z }{m_c^*}\hat z$$
Recall $J=nqv=\frac{qg_sg_v}{L^3}\sum_kf(k)v_g(k)$,
$$J_{s\to m} = \frac{qg_sg_v}{L^3} \sum_k \frac{\hbar k_z }{m_c^*}\frac{1}{1 + \exp(\frac{\hbar k^2}{2m_c^*kT}-\frac{E_{fs}-E_c(0)}{kT})}$$
Assume it's sufficiently large crystal, so we can approximate with an integral in spherical coordinates,
$$J_{s \to m} = \int \int \int \frac{k_zk^2\sin\theta }{(\frac{2\pi}{L})^3}\frac{1}{1 + \exp(\frac{\hbar k^2}{2m_c^*kT}-\frac{E_{fs}-E_c(0)}{kT})}dk d\theta d\phi$$
$$J_{s \to m} = \int \int \int \frac{k^3\cos\theta \sin\theta }{(\frac{2\pi}{L})^3}\frac{1}{1 + \exp(\frac{\hbar k^2}{2m_c^*kT}-\frac{E_{fs}-E_c(0)}{kT})}dk d\theta d\phi$$
Where the $(\frac{2\pi}{L})^3$ term arises trivially by the $dk$ term contributions. Since it is a hemisphere, we naturally set the bounds:
$$J_{s \to m} = \int_0^k \int_0^\pi \int_0^\pi \frac{k^3\cos\theta \sin\theta }
{(\frac{2\pi}{L})^3}\frac{1}{1 + \exp(\frac{\hbar k^2}{2m_c^*kT}-\frac{E_{fs}-E_c(0)}{kT})}dk d\theta d\phi$$

Solving the angular terms and taking a u-sub of the k term,
$$J_{s \to m} = \frac{2\pi q g_sg_vm_c^*k_B^2}{h^3}T^2\int_0^\infty \frac{udu}{1 + \exp(\frac{\hbar k^2}{2m_c^*kT}-\frac{E_{fs}-E_c(0)}{kT})}$$
We recognize the coefficient as the richardson constant
$$A^*=\frac{2\pi q g_sg_vm_c^*k_B^2}{h^3}$$
$$J_{s \to m} = A^*T^2\int_0^\infty \frac{udu}{1 + \exp(\frac{\hbar k^2}{2m_c^*kT}-\frac{E_{fs}-E_c(0)}{kT})}$$
We recognize the integral as a fermi integral, $F_j$, specifically $F_1$. 
$$F_j = \frac{1}{\Gamma(j+1)}\int_0^\infty \frac{u^jdu}{1 + \exp(\frac{\hbar k^2}{2m_c^*kT}-\frac{E_{fs}-E_c(0)}{kT})}$$
$$\implies  J_{s \to m} = A^*T^2F_1(\eta_s), \ \ \ \eta_s = \frac{E_{fs}-E_c(0)}{k_BT}$$
By symmetry, we can see that the mirror version (done on the other hemisphere for electrons moving to the semiconductor) is,
$$J_{m \to s} = A^*T^2F_1(\eta_m), \ \ \ \eta_m = \frac{E_{fm}-E_c(0)}{k_BT}$$
The net current is then,
$$J_{net} = J_{s \to m} - J_{m \to s} = A^*T^2(F_1(\eta_s) -F_1(\eta_m))$$
We know that $\eta_s, \eta_m << -1$ by inspection of their definitions. As a result, we can approximate the fermi integral as 
$$F_1(\eta) = \frac{1}{\Gamma(2)} \int_0^\infty \frac{udu}{1 + \exp(\frac{\hbar k^2}{2m_c^*kT}-\frac{E_{fs}-E_c(0)}{kT})} \approx e^{\eta}$$
Therefore, 
$$J_{net} = J_{s \to m} - J_{m \to s} = A^*T^2(e^{\eta_s} -e^{\eta_m})$$
Recall $q\phi_B = qV_a+(E_c(0)-E_F)$. So,
$$\eta_m  = -\frac{q\phi_b}{kT}, \ \ \ \eta_s = -\frac{q\phi_B - qV_A}{kT}$$
$$J_{net} = J_{s \to m} - J_{m \to s} = A^*T^2(e^{-\frac{q\phi_B - qV_A }{kT}} -e^{-\frac{q\phi_BZ}{kT}})$$
$$\boxed{J_{net}=A^*T^2e^{-\frac{q\phi_B}{kT}}(e^{\frac{qV_A}{kT}}-1)}$$
