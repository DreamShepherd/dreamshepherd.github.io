Bryant Har
### Q1
###### Part A
We begin and add $m_1r_2-m_1r_2$ to left side,
$$m_1r_1+m_2r_2=0$$
$$m_1r_1-m_1r_2+(m_1+m_2)r_2=0 \implies r_2=-\frac{m_1r}{m_1+m_2}$$
By symmetry, $$\boxed{\vec{r_1}=\frac{m_2\vec{r}}{m_1+m_2},\ \ \ \vec{r_2}=-\frac{m_1\vec{r}}{m_1+m_2}}$$
###### Part B
We begin,
$$l = r_1 \times m_1\dot r_1 + r_2 \times m_2 \dot r_2$$
$$l = 
\frac{m_2\vec{r}}{m_1+m_2}
\times 
\frac{m_1m_2 \dot{\vec{r}}}{m_1+m_2}
-\frac{m_1\vec{r}}{m_1+m_2}
\times 
-\frac{m_1m_2\dot{\vec{r}}}{m_1+m_2}$$
Substituting reduced mass $\mu$,
$$l = 
\frac{m_2\vec{r}}{m_1+m_2}
\times 
\mu\dot{\vec{r}}
+\frac{m_1\vec{r}}{m_1+m_2}
\times 
\mu\dot{\vec{r}}= 
\frac{(m_1+m_2)\vec{r}}{m_1+m_2}
\times 
\mu\dot{\vec{r}}$$
$$=\boxed{\vec{\ell}=\vec{r} \times \mu \dot{\vec{r}}}$$
This is clearly displacement cross momentum, as expected.
###### Part C
Referencing b, we have displacement cross momentum, where momentum is $\mu \dot r$. Inspecting our momentum term, we clearly see that $\mu$ is the one-particle mass analogue,
$$\boxed{\mu \text{, the reduced mass}}$$
###### Part D
We begin,
$$\dot{\vec{r}}=\dot r \hat r + r \dot \theta \hat \theta$$
Recall that $\vec{l}=r \hat r \times \mu \dot{\vec{r}}$. We arrive at $\dot \theta$ in terms of $\ell$,
$$\frac{l}{\mu r } = r |\dot \theta| \implies |\dot \theta| = \frac{\ell}{\mu r^2} \implies \boxed{\dot \theta = \frac{\pm \ell}{\mu r^2}}$$
### Q2
###### Part A
$$E=\frac{1}{2}\mu \dot r ^2 + \frac{1}{2}\frac{\ell^2}{\mu r ^2}+U(r)$$
Rewriting,
$$\boxed{
\dot r = \sqrt{\frac{2(E-U(r))}{\mu}-\frac{\ell^2}{\mu^2 r ^2}}
}$$
###### Part B
From a and d,
$$\dot \theta = \pm \frac{\ell}{\mu r^2},\ \ \ \dot r = \sqrt{\frac{2(E-U(r))}{\mu}-\frac{\ell^2}{\mu^2 r ^2}}
$$
Now, combining into the given equation in b,
$$d\theta = \frac{\dot \theta}{\dot r} dr = \frac{\pm \frac{\ell}{\mu r^2}}{\sqrt{\frac{2(E-U(r))}{\mu}-\frac{\ell^2}{\mu^2 r ^2}}}dr=\frac{\pm \frac{\ell}{ r^2}}{\sqrt{2\mu^2\big[\frac{E-U(r)}{\mu}-\frac{\ell^2}{2\mu^2 r ^2}\big]}}dr$$
As desired, we arrive at
$$\implies \boxed{\theta = \int\frac{\pm \frac{\ell}{ r^2}}{\sqrt{2\mu\big[E-U(r)-\frac{\ell^2}{2\mu r ^2}\big]}}dr}$$
###### Part C
We replace $U$,
$$\theta = \int\frac{\pm \frac{\ell}{ r^2}}{\sqrt{2\mu\big[E-U(r)-\frac{\ell^2}{2\mu r ^2}\big]}}dr=\int\frac{\frac{\ell}{ r^2}}{\sqrt{2\mu\big[E+k/r-\frac{\ell^2}{2\mu r ^2}\big]}}dr$$
Note that, $u=l/r \implies du = -l/r^2 dr$. Making this sub and absorbing the numerator into du, 
$$\theta = \int\frac{-du}{\sqrt{2\mu\big[E+ku/l-\frac{u^2}{2\mu}\big]}}=\boxed{\theta=\int\frac{-du}{\sqrt{2\mu E+2\mu ku/l-u^2}}}$$
###### Part D
We begin,
$$\theta=\int\frac{-du}{\sqrt{2\mu E+2\mu ku/l-u^2}}$$
Applying the arcsin formula,
$$\int \frac{du}{\sqrt{au^2+bu+c}}=-\frac{1}{\sqrt{-a}}\arcsin(\frac{2au+b}{\sqrt{b^2-4ac}})$$
$$
\theta=\int\frac{-du}{\sqrt{2\mu E+2\mu ku/l-u^2}}

=

\arcsin(\frac{-2u+2\mu k/l}{\sqrt{(2\mu k/l)^2+4(2\mu E)}})
+C_1$$
Reorganizing terms, we arrive as desired
$$\boxed{\theta + C= \arcsin(\frac{-2u+2\mu k/l}{\sqrt{(2\mu k/l)^2+8\mu E}})}$$
###### Part E
$\fbox{Yes}$. 
$\sin(\theta-\pi/2)=-\cos(\theta) \implies \theta -\pi/2 = \arcsin(\xi) \implies \theta = -\arccos(\xi)$. 
To adjust for $\arccos$ limits, we can add a factor of $\pi$,
$$\boxed{\theta = \pi-\arccos(\frac{-2u+2\mu k/l}{\sqrt{(2\mu k/l)^2+8\mu E}})}$$
###### Part F
Letting $\epsilon \equiv \sqrt{1+\frac{2El^2}{\mu k^2}},\ \ \ c \equiv \frac{l^2}{\mu k},\ \ \ u=l/r$, starting from above,
$$\pi-\theta = \arccos(\frac{-2u+2\mu k/l}{\sqrt{(2\mu k/l)^2+8\mu E}}) \implies \cos(\pi-\theta)=-\cos(\theta)=
\frac{-ul/\mu k+1}{\epsilon}=
\frac{-c/r+1}{\epsilon}$$
$$-\cos(\theta)=\frac{-c/r+1}{\epsilon} \implies \boxed{r=\frac{c}{1+\epsilon \cos(\theta)}}$$
###### Part G
$$r=\frac{c}{1+\epsilon \cos(\theta)}$$
Clearly minimized when cosine is zero, or $\boxed{\theta = \pi/2}$
### Q3
###### Part A
$\fbox{Momentum}$ (or generalized momentum)
###### Part B
Zero when different indices, one when same.
$$\boxed{\frac{\partial \dot q_k}{\partial \dot q_i} =\delta_{ik}}$$
###### Part C
$$\frac{dy_a}{dt}=\sum_j\frac{dy_a}{dq_j}\dot q_j+\frac{dy_a}{dt}$$
Taking the derivative wrt $\dot q_i$,
$$\frac{d \dot y_a}{d \dot q_i}=\sum_j\frac{d}{d\dot q_i}(\frac{dy_a}{dq_j}\dot q_j)+\frac{dy_a}{d\dot qdt}$$
Since $y_a$ has no time derivative dependence, and by symmetry of second derivatives, we can pull out the inner derivative (independent wrt to $\dot q$) and set the outer derivative term to zero,
$$\frac{d \dot y_a}{d \dot q_i}=\sum_j\frac{d\dot q_j}{d\dot q_i}\frac{dy_a}{dq_j}+0=\frac{dy_a}{dq_j}\delta_{ij}$$
$$\boxed{\frac{d \dot y_a}{d \dot q_j}=\frac{dy_a}{dq_j}}$$
###### Part D
Substitute $-\nabla U = F_{tot}-F_c$. Then, constraint is holonomic. Then, follows newton's second. We apply these constraints in that order below, line by line,
$$\delta S = \int_{t_1}^{t_2}(-m\ddot  {\vec r} - \vec \nabla U) \cdot \delta \vec r dt =\int_{t_1}^{t_2}(-m\ddot  {\vec r} +F_{tot}-F_{c}) \cdot \delta \vec r dt $$
$$F_c \cdot \delta r = 0 \implies \delta S  =\int_{t_1}^{t_2}(-m\ddot  {\vec r} +F_{tot}) \cdot \delta \vec r dt $$
$$
m\ddot{\vec r} = F_{tot} \implies \delta S  =\int_{t_1}^{t_2}(-F_{tot}+F_{tot}) \cdot \delta \vec r dt =\int_{t_1}^{t_2}(0)dt=\delta S=0
$$
If these above conditions hold, we arrive that $\delta S$ must be zero as the integrand is zero and by fundamental theorem of calculus.
$$\boxed{\delta S=0} $$$$\boxed{\text{Q.E.D.}}$$
###### Part E
$\fbox{Yes}$ they would be smooshing in momentum axis $p_x$.
###### Part F
$\fbox{Yes}$. Like mixing colors, points may end up with different sets of neighbors as the phase space evolves.
### Q4
###### Part A
$\boxed{\text{No. } H \neq T+U }$. The accelerating pivot introduces energy into the system and therefore the Hamiltonian is not just the sum kinetic energy and potential energy. It must also include terms that incorporate the motion of the pivot.
###### Part B
Recall the Lagrangian found earlier.
$$
\mathcal{L}=\frac{M}{2}(a^2t^2+atL\cos(\theta)\dot \theta+\frac{L^2\dot \theta^2}{3})+MgL\cos(\theta)/2
$$
Finding the momenta,
$$p_\theta=\frac{d\mathcal{L}}{d\dot \theta}=\frac{MatL\cos(\theta)}{2}+
\frac{ML^2\dot \theta}{3}
\implies \dot \theta = \frac{3p_\theta}{ML^2} - \frac{3at\cos(\theta)}{2L}$$
Then,
$$\mathcal{H}=\dot \theta p_\theta - \mathcal L=
\frac{MatL\cos(\theta)\dot \theta}{2}+
\frac{ML^2\dot \theta^2}{3}
-
\frac{M}{2}(a^2t^2+atL\cos(\theta)\dot \theta+\frac{L^2\dot \theta^2}{3})-MgL\cos(\theta)/2
$$
$$
\mathcal{H}=\dot \theta p_\theta - \mathcal L=

\frac{ML^2\dot \theta^2}{6}
-
\frac{Ma^2t^2}{2}-MgL\cos(\theta)/2$$
$$\boxed{\mathcal H=

\frac{ML^2}{6}
\bigg(
\frac{3p_\theta}{ML^2} - \frac{3at\cos(\theta)}{2L}
\bigg)^2
-
\frac{Ma^2t^2}{2}-MgL\cos(\theta)/2

}$$
### Q5
###### Part A
We find the Hamiltonian. While we can first find the lagrangian and then derive the Hamiltonian, we can more easily consider the hamiltonian as the total energy of the inertial system. Notice that $r=L$ by constrains $r$, so we only require two coordinates.
$$T=\frac{1}{2}mv^2,\ \ \ U=mgL\cos \theta$$
$$\mathcal L=T-U=\frac{1}{2}mL^2(\dot \theta^2+\sin^2(\theta)\dot\phi ^2)-mgL\cos(\theta)$$
$$p_\phi =\frac{\partial \mathcal L}{\partial \dot\phi}=mL^2\sin^2(\theta)\dot\phi$$$$ p_\theta =\frac{\partial \mathcal L}{\partial \dot\theta}= mL^2\dot \theta$$$$
H=\frac{1}{2}mv^2+mgL\cos(\theta)=\frac{1}{2}m(L^2\dot \theta^2+L^2\sin^2(\theta)\dot\phi ^2)+mgL\cos(\theta)$$Substituting our momenta identities,
$$\boxed{\mathcal H=\frac{p_\theta^2}{2mL^2}+\frac{p_\phi ^2}{2mL^2\sin^2 \theta}+mgL\cos(\theta)}$$
###### Part B
$$\frac{\partial H}{\partial \theta}=-\dot p_\theta \ \ \ \frac{\partial H}{\partial \phi}=-\dot p_\phi \ \ \ \frac{\partial H}{\partial p_\theta}=\dot \theta,\ \ \ \ \frac{\partial H}{\partial p_\phi}=\dot \phi$$
We substitute and take the partial derivatives as necessary to generate the above equations. We arrive at four relevant equations,
$$
\boxed{\dot p_\phi = 0,\ \ \ \ \ \ \  \dot \phi = \frac{p_\phi }{mL^2\sin^2 \theta}}
$$
$$
\boxed{\dot p_\theta = 
mgL\sin \theta

+\frac{p_\theta ^2}{mL^2\sin^2 \theta}\cot\theta
,\ \ \ \ \ \ \ 
\dot \theta = 
\frac{p_\theta }{mL^2}
}$$
The momentum equations relating $\dot q$ and $p_q$ (second equations in each pair) are equivalent to the ones ones written when developing.
###### Part C
$\boxed{\phi \text{ direction}}$.
$\boxed{\text{There is rotational symmetry in the } \phi \text{ direction}}$.
$p_\phi$ must be constant since time derivative is zero.
###### Part D
$\boxed{-\hat \theta \text{ direction}}$

Recall we defined $p_\phi =mL^2\sin^2(\theta)\dot\phi$.
Inspecting the definition given, this corresponds to $L \cdot (-\hat \theta)$, or the $-\hat \theta \text{ direction}$
###### Part E
$$p_\theta =mL^2\dot \theta \implies \dot p_\theta =mL^2\ddot \theta$$
$$\dot p_\theta = 
mgL\sin \theta+\frac{p_\phi ^2}{mL^2\sin^2 \theta}\cot \theta$$
$$\boxed{mL^2\ddot \theta=\frac{p_\phi ^2}{mL^2\sin^2 \theta}\cot \theta+mgL\sin \theta}$$
