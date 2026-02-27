#physics 
# Q1
### Q1a
We begin,
$$\mathcal L = \frac{1}{2}\mu (\dot r ^2 +r^2\dot \phi^2) - k r^n$$
By Euler-Lagrange,
$$0 = \frac{d \mathcal L }{d\phi}-\frac{d}{dt}\frac{d \mathcal L}{d\dot \phi} = 
\boxed{-\mu r^2 \ddot \phi = 0}
$$
$$0 = \frac{d \mathcal L }{dr}-\frac{d}{dt}\frac{d \mathcal L}{d\dot r} = \mu r \dot \phi^2-knr^{n-1}-\mu \ddot r \implies \boxed{\mu \ddot r = \mu r \dot \phi^2-knr^{n-1}}$$
Combining, 
$$\dot \phi = \frac{L}{\mu r^2} \implies \boxed{

\mu \ddot r = 
\frac{L^2}{\mu r^3}
-knr^{n-1}

}$$
### Q1b
If $\dot r = 0$, then
$$\mu \ddot r = 
\frac{L^2}{\mu r^3}
-knr^{n-1}=0 \implies 

\boxed{
r_{eq} = 
\bigg(
\frac{ L^2}{kn\mu}
\bigg)^{1/(n+2)}
}$$
### Q1c
We have
$$\mu \ddot r = 
\frac{L^2}{\mu r^3}
-knr^{n-1}
,\ \ \ kn>0$$
To be stable, this derivative must be negative at equilibrium.
$$
-\frac{3L^2}{\mu r_{eq}^4}
-kn(n-1)r_{eq}^{n-2}<0
,\ \ \ kn>0
$$
$$
-\frac{3L^2}{\mu}
<kn(n-1)r_{eq}^{n+2}=kn(n-1)\frac{L^2}{kn\mu}\implies -3<n-1
$$
$$-3<n-1 \implies \boxed{-2<n} \implies \boxed{\text{stable for\ }n\geq-1}$$
### Q1d
Using the above derivative as $-k$ for hooke's law,
$$\tau_{osc} = 2\pi/ \omega,\ \ \ \omega^2 = k/\mu=

\frac{3L^2}{\mu^2 r_{eq}^4}
+\frac{kn(n-1)}{\mu}r_{eq}^{n-2}

$$
$$\boxed{\tau_{osc}=\frac{2\pi}{\sqrt{\frac{3L^2}{\mu^2 r_{eq}^4}
+\frac{kn(n-1)}{\mu}r_{eq}^{n-2}
}}}$$
### Q1e
Recall $\dot \phi = \frac{L}{\mu r^2}$. Period is $2\pi/\dot\phi$
$$ \implies \boxed{
\tau_{orb}=\frac{2\pi\mu}{L}
\bigg(
\frac{ L^2}{kn\mu}
\bigg)^{2/(n+2)}
}
$$
### Q1f
We require rational square root values. We use $\sqrt{n+2}=1,2,3$. Then,
$$\boxed{n=-1,2, 7}$$
# Q2

***Big sun mass and radius approximations used throughout.***

### Q2a
By formula, we have
$$E=-\frac{GM_sm}{2a} \implies \boxed{E=-\frac{GM_Sm}{2r_1}}$$
### Q2b
Kinetic energy is 
$$T=E-U=-\frac{GM_s}{2r_1}+\frac{GM_s}{r_1}=
\frac{GM_sm}{2r_1}=\frac{1}{2}mv_1^2$$
$$\implies \boxed{
v_1=\sqrt{\frac{GM_s}{r_1}}
}
$$
### Q2c
The energy of the orbital transfer is the change in energy:
$$
\boxed{\Delta E=
\frac{GM_Sm}{2}\bigg(
\frac{1}{r_1}-\frac{1}{r_2}
\bigg)
}
$$
### Q2d
$\fbox{Parallel to original velocity (tangential)}$
We expand,
$$|v_1+\Delta v|^2=|v_1|^2+2v_1 \cdot \Delta v + |\Delta v|^2$$
To maximize the post-impulse speed, we must maximize the $2v_1 \cdot \Delta v$ term. Clearly, this dot product is maximized when $\Delta v$ is tangential or parallel to $v_1$.
### Q2e
At maximum, $a= \frac{r_1+r_2}{2}$. By energy conservation,
$$-\frac{GM_sm}{r_1}+\frac{1}{2}mv_f^2=-\frac{GM_sm}{r_1+r_2} \implies v_f = \sqrt{2GM_s(\frac{1}{r_1}-\frac{1}{r_1+r_2})}$$
$$v_f-v_1=\boxed{\Delta v = \sqrt{\frac{2GM_s}{r_1}-\frac{2GM_s}{r_1+r_2}}-\sqrt{\frac{GM_s}{r_1}}}$$
### Q2f
By Kepler's third,
$$\frac{a^3}{T^2}=\frac{G(M+m)}{4\pi^2}\approx\frac{GM_s}{4\pi^2}\implies T = \sqrt{\frac{4\pi^2(r_1+r_2)^3}{2^3GM_s}}=\sqrt{\frac{\pi^2(r_1+r_2)^3}{2GM_s}}$$
We only complete a half period, so we coast for
$$\boxed{
t=\sqrt{\frac{\pi^2(r_1+r_2)^3}{8GM_s}}
 }\text{ units of time}$$
### Q2g
$\fbox{Yes}$. Otherwise you'll have to catch up/slow down to Venus.
### Q2h
$\fbox{Yes}$. Otherwise you'll continue moving in the elliptical non-Venus orbit.
# Q3
### Q3a
By formula, we have for rocket mass $m_r$,
$$E=
\frac{1}{2}m_r(7900)^2-\frac{GMm_r}{6371000+250000}
=−28997770.6691\cdot m_r$$
$$\boxed{E=m_r \cdot-2.9\cdot10^{7}
 } \text{ Joules}$$
where $m_r$ is numerical rocket mass in kgs but carries no units.
### Q3b
We have,
$$E=-\frac{GMm}{2a}=m \cdot-2.9\cdot10^{7}$$
$$\implies a = 6872986m \approx \boxed{a=6.87\cdot 10^6 \text{ meters}}$$
### Q3c
By Kepler's Third,
$$\frac{a^3}{T^2}=\frac{G(M+m)}{4\pi^2}\approx\frac{GM_s}{4\pi^2} \implies T=2\pi\sqrt{\frac{a^3}{GM_E}}=5666.892s$$
$$\boxed{T=5667 \text{ seconds}}$$
### Q3d
We already found $a$ is 6872986m. The closest distance is the initial distance or $6371000+250000=6621000$ meters. This must be either the closest or the farthest, since the rocket is perpendicular to the earth at this point. Since it's less than $a$, it is the closest. The farthest, then, must be at a distance

$$r_{close} = 6621000 \implies r_{far}=(a-r_c)+a = 7119000$$
$$\boxed{
r_{close} = 6.62\cdot 10^6 \text{ meters}, \ \ \
r_{far} = 7.12\cdot 10^6 \text{ meters}
}$$
### Q3e
By formula,
$$\epsilon = \frac{r_{far} - r_{close}}{r_{far}+r_{close}}=0.0363901018923$$
$$\boxed{\epsilon = 0.0364}$$
So it is fairly circular.
### Q3f
$\boxed{28.5 \text{ degrees}}$
They are the same. 