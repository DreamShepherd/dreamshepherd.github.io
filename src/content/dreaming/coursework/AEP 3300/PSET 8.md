Bryant Har

### Q1
###### Part A
Set origin to be (0,0). Then, 
$U = mgy_m = mg(l-l\cos(\theta))$
Using the image we find velocity by differentiation,
$$(x_m, y_m)= (A\sin(\omega t) + l\sin(\theta), l-l\cos(\theta))$$
$$\implies (\dot x_m, \dot y_m)= (A\omega\cos(\omega t) + l\cos(\theta)\dot \theta, l\sin(\theta)\dot \theta) $$
$$\implies v^2=
(A\omega\cos(\omega t) + l\cos(\theta)\dot \theta)^2 +(l\sin(\theta)\dot \theta)^2
$$
We then find kinetic energy,
$$T=\frac{1}{2}m\big[
(A\omega\cos(\omega t) + l\cos(\theta)\dot \theta)^2 +(l\sin(\theta)\dot \theta)^2
\big]$$
$$T=\frac{1}{2}m\big[
A^2\omega^2\cos^2(\omega t) + 2A\omega l \dot \theta \cos(\omega t)\cos(\theta)+l^2\dot \theta^2
\big]$$
The lagrangian, then, is
$$\implies L = T-U$$
$$\boxed{\mathcal{L}=
\frac{1}{2}m\big[
A^2\omega^2\cos^2(\omega t) + 2A\omega l \dot \theta \cos(\omega t)\cos(\theta)+l^2\dot \theta^2
\big]
-mg(l-l\cos(\theta))}$$
###### Part B
We begin with EL
$$0=\frac{dL}{d\theta}-\frac{d}{dt}\frac{dL}{d \dot\theta}$$
Substituting L,
$$0=-mA\omega l\dot \theta \cos(\omega t)\sin(\theta)-mgl\sin(\theta)
-ml^2\ddot \theta+mA\omega l\cos(\omega t)\sin(\theta)\dot \theta + mA\omega^2 l\sin(\omega t)\cos(\theta)$$
We rearrange and cancel terms to arrive at our final EL equation,
$$\boxed{
ml^2\ddot \theta=mlA\omega^2 \sin(\omega t)\cos(\theta)-mlg\sin(\theta)

}$$
$$\boxed{
l\ddot \theta=A\omega^2 \sin(\omega t)\cos(\theta)-g\sin(\theta)}$$
###### Part C
The mass must obey circular motion, so 
$$(x-x_p)^2+(y-y_p)^2=l^2$$
$$(x-A\sin(\omega t))^2+(y-l)^2=l^2$$
We arrive,
$$\boxed{
(x-A\sin(\omega t))^2+(y-l)^2-l^2=0
}$$
###### Part D
We have two time-dependent coordinates, x and y, but we can eliminate one with the constraint, so
$$2-1=\boxed{\text{1 degree of freedom}}$$
###### Part E
From part A, we found by looking at the diagram,
$$(x_m, y_m)= (A\sin(\omega t) + l\sin(\theta), l-l\cos(\theta))$$
Decomposing,
$$\boxed{\begin{cases}
x=A\sin(\omega t)+l\sin(\theta)\\
y = l - l\cos(\theta)
\end{cases}}$$
### Q2
###### Part A
$\fbox{One degree of freedom}$
($\theta$, the constant acceleration is not free since always known)
###### Part B

**We use the origin as the horizontal line at the pivot. x,y is at some point l away.**
$$(x,y) = (\frac{a}{2}t^2+l\sin(\theta), -l\cos(\theta))$$
Potential energy is simply gravity at com,
$$U = Mgy=-MgL/2\cos(\theta)$$
We find velocity by differentiation,
$$(\dot x,\dot y) = (at+l\cos(\theta)\dot \theta,l\sin(\theta)\dot \theta)$$
$$\implies v^2=\big[(at+l\cos(\theta)\dot \theta)^2+ (l\sin(\theta)\dot \theta)
)^2\big]=a^2t^2+2atl\cos(\theta)\dot \theta+l^2\dot \theta^2$$
Starting from first principles, we find KE, integrating over length using uniform mass density.
Notably, this method will intrinsically incorporate **BOTH rotational and translational KE**
$$T=\frac{1}{2}\int v^2 dm=\frac{M}{2L}\int_0^L \big[a^2t^2+atl\cos(\theta)\dot \theta+l^2\dot \theta^2]dl$$
$$T=\frac{M}{2}(a^2t^2+atL\cos(\theta)\dot \theta+\frac{L^2\dot \theta^2}{3})$$
Then,
$$T-U=\boxed{\mathcal{L}=\frac{M}{2}(a^2t^2+atL\cos(\theta)\dot \theta+\frac{L^2\dot \theta^2}{3})+MgL\cos(\theta)/2}$$
###### Part C
We begin, with the general form. (I write $\mathcal{L}$ as L for convenience, not to be confused for the rod length)
$$0=\frac{dL}{d\theta}-\frac{d}{dt}\frac{dL}{d \dot\theta}$$
$$\frac{dL}{d\theta}=-MgL\sin(\theta)/2-MatL\sin(\theta)\dot \theta/2$$
$$\frac{d}{dt}\frac{dL}{d \dot\theta}=MaL\cos(\theta)/2-MatL\sin(\theta)\dot\theta/2+ML^2\ddot \theta/3$$
$$0=\frac{dL}{d\theta}-\frac{d}{dt}\frac{dL}{d \dot\theta}$$
$$=
-MgL\sin(\theta)/2-MatL\sin(\theta)\dot \theta/2-(MaL\cos(\theta)/2-MatL\sin(\theta)\dot\theta/2+ML^2\ddot \theta/3)$$
Canceling,
$$0=\frac{dL}{d\theta}-\frac{d}{dt}\frac{dL}{d \dot\theta}=-MgL\sin(\theta)/2-MaL\cos(\theta)/2-ML^2\ddot \theta/3$$$$\boxed{\frac{2}{3}L\ddot \theta=-g\sin(\theta)-a\cos(\theta)}$$
###### Part D
In equilibrium, $\ddot \theta=0$. Then, using the above equation,
$$g\sin\theta=-a\cos(\theta)$$
$$\implies \boxed{\theta_{eq} = -\arctan(a/g)}$$
In equilibrium, the pivot dangles behind the direction of constant acceleration

###### Part E
To be stable, we require the potential to be convex or bowl-shaped minimum at that point. 
$$\boxed{\frac{d^2 U}{dt^2}(\theta_{eq})>0 \text{ and } \frac{dU}{dt}=0}$$
Additionally to follow hooke's law, we require the coeffiicent to be negative for stable equilibrium.
Looking at the result below, this is stable if
$$\text{consequently requires\  \ \ }\boxed{ g\cos(\theta_{eq})-a\sin(\theta_{eq})>0}$$
Equivalently for small oscillations, we can show it follows hooke's law. We do this in the next part.
$$m\ddot q=-kq$$
Here is a sketch of the stick at a stable equilibrium. The pivot is accelerating to the right and the tip is pulled down by gravity. In the end, it dangles behind.
![[Pasted image 20231019131347.png | 500]]
We clearly see by the sketch that the bar cannot go above the horizontal in equilibrium. Gravity would pull it down. 
The angle absolute value must be $\boxed{\text{less than } \pi/2}$.
###### Part F
We seek a hooke's law relation to derive frequency. We arrived at the EOM,
$$\ddot \theta=-\frac{3g}{2L}\sin(\theta)-\frac{3a}{2L}\cos(\theta)$$
For small angles (oscillations) at equilibrium,
$$\theta \approx \theta_{eq}+\delta\theta$$
$$\ddot \theta=-\frac{3g}{2L}\sin(\theta)-\frac{3a}{2L}\cos(\theta)\approx-\frac{3g}{2L}\sin(\theta_{eq}+\delta\theta)-\frac{3a}{2L}\cos(\theta_{eq}+\delta\theta)$$
$$=-\frac{3g}{2L}\big[\sin(\theta_{eq})\cos(\delta\theta)+\cos(\theta_{eq})\sin(\delta\theta)\big]-\frac{3a}{2L}(\big[\cos(\theta_{eq})\cos(\delta\theta)-\sin(\theta_{eq})\sin(\delta\theta)\big])$$
Applying small angle,
$$\ddot \theta \approx-\frac{3g}{2L}\big[\sin(\theta_{eq})+\cos(\theta_{eq})\delta\theta\big]-\frac{3a}{2L}(\big[\cos(\theta_{eq})-\sin(\theta_{eq})\delta\theta\big])$$
Comparing the linear term coefficient to hooke's law $\ddot q=-\omega^2 q$, we arrive at
$$\omega^2 \approx \frac{g\cos(\theta_{eq})-a\sin(\theta_{eq})}{2L/3}$$
$$\implies \boxed{\omega = \sqrt{\frac{g\cos(\theta_{eq})-a\sin(\theta_{eq})}{2L/3}}}$$
### Q3
###### Part A
$$g=-g\langle 0,1/2,\sqrt{3}/2\rangle$$
We have three degrees of freedom. We define coordinate $x=X_b, y=Y_b, \theta$. $x,y$ is the center of the stick, and $\theta$ is the angle of the stick taken CCW from the positive x direction. We also assume all coordinates start at 0 as origin. We also let the board decline in the y direction. We now find the Lagrangian.

We find kinetic energy. This is rotational and translational. 
$$T_M=\frac{1}{2}M\dot x^2+\frac{1}{2}M\dot y^2+\frac{1}{2\cdot 12}ML^2\dot\theta^2$$
Letting $R=L/2$,
$$(x_m,y_m)=(x+R\cos(\theta), y+R\sin(\theta))$$
$$T_m=\frac{1}{2}mR^2\dot\theta^2+\frac{1}{2}m(\dot x-R\sin(\theta)\dot\theta)^2+\frac{1}{2}m(\dot y+R\cos(\theta)\dot\theta)^2$$
We can make the calculations more tractable by simply considering their translational velocities to be the same.
$$T_m=\frac{1}{2}mR^2\dot\theta^2+\frac{1}{2}m\dot x^2+\frac{1}{2}m\dot y^2$$
Potential is simply gravity. We define it as zero at origin.
$$U(y)=-Mgy\frac{\sqrt{3}/2}{1/2}=-\frac{Mgy}{\sqrt{3}}$$
$$\implies U=-\frac{Mgy}{\sqrt{3}}-\frac{mg(y+R\sin(\theta))}{\sqrt{3}}$$
Together, we get
$$\mathcal{L}=T-U=$$
$$
\mathcal{L}=\frac{1}{2}(M+m)\dot x^2+\frac{1}{2}(M+m)\dot y^2+\frac{1}{24}ML^2\dot\theta^2+\frac{1}{2}mR^2\dot\theta^2+\frac{Mgy}{\sqrt{3}}+\frac{mg(y+R\sin(\theta))}{\sqrt{3}}$$
Replacing R,
$$\boxed{\mathcal{L}=\frac{1}{2}(M+m)\dot x^2+\frac{1}{2}(M+m)\dot y^2+\frac{1}{24}ML^2\dot\theta^2+\frac{1}{8}mL^2\dot\theta^2+\frac{Mgy}{\sqrt{3}}+\frac{mg(y+L\sin(\theta)/2)}{\sqrt{3}}}$$

###### Part B
We again use
$$0=\frac{dL}{dx}-\frac{d}{dt}\frac{dL}{d \dot x}$$
X derivative is zero. We compute the rest
$$\implies (M+m)\ddot x = 0$$
In y direction, we compute all the partials as none are zero
$$0=\frac{dL}{dy}-\frac{d}{dt}\frac{dL}{d \dot y}$$
$$\frac{(M+m)g}{\sqrt{3}}-(M+m)\ddot y=0$$
In theta direction,
$$0=\frac{dL}{d\theta}-\frac{d}{dt}\frac{dL}{d \dot \theta}$$
The theta derivative is zero, we compute the rest
$$0= (\frac{1}{12}ML^2+\frac{1}{4}mL^2)\ddot \theta$$
Collecting our EOM,
$$
\boxed{\begin{cases}
(\frac{1}{12}ML^2+\frac{1}{4}mL^2)\ddot \theta=0\\
(M+m)\ddot x = 0\\
\ddot y=\frac{g}{\sqrt{3}}
\end{cases}}
$$
We can remove the constant terms for the values that equal zero if desired, though some meaning is lost.
$$
\boxed{\begin{cases}
\ddot \theta=0\\
\ddot x = 0\\
\ddot y=\frac{g}{\sqrt{3}}
\end{cases}}
$$
###### Part C
We see that the theta and x direction EOM are zero. This means there is symmetry along theta and x. Specifically, these refer to
$$\fbox{Linear momentum conserved along x direction. Symmetry of translation along x direction.}$$
$$\fbox{Angular momentum conserved. Symmetry along theta direction (stick spinning)}$$
###### Part D
From taylor,
$$R=\frac{M_1R_1+M_2R_2}{M_1+M_2}$$
Let 1 be the large stick and 2 be the point mass. $R_1$ averages out to $0$. $R_2$ is $L/2$.
Then, we have
$$R=\frac{0+mL/2}{M+m}$$$$\boxed{R=\frac{mL}{2(M+m)}}$$
as desired


### Q4
###### Part A
$\fbox{Two degrees of freedom}$. One for small and large disk. Let the small disk be $\phi$ and the large disk be $\theta$, both measured CCW from the downward vertical.

###### Part B
We define kinetic energy,
$$T=\frac{1}{2}M(R\dot\theta)^2+\frac{1}{2}\frac{1}{2}MR^2\dot\theta^2+\frac{1}{2}m\big(\frac{R\dot\theta}
{2}\big)^2+\frac{1}{2}\frac{1}{2}m(\frac{R}{4})^2\dot\phi^2$$
$$T=\frac{1}{2}M(R\dot\theta)^2+\frac{1}{4}MR^2\dot\theta^2+\frac{1}{8}m(R\dot\theta)^2+\frac{1}{64}mR^2\dot\phi^2$$
$$T=(\frac{3}{4}MR^2+\frac{1}{8}mR^2)\dot\theta^2+\frac{1}{64}mR^2\dot\phi^2$$
We find potential energy,
$$U=MgR+mg(R-\frac{R}{2}\cos(\theta))$$
The lagrangian, then, is
$$\mathcal{L} = T-U=\boxed{\mathcal{L} =(\frac{3}{4}MR^2+\frac{1}{8}mR^2)\dot\theta^2+\frac{1}{64}mR^2\dot\phi^2-(MgR+mg(R-\frac{R}{2}\cos(\theta)))}$$
###### Part C
We write out the EL for the first equation wrt to the little disk angle $\phi$ using the above lagrangian and computing partials,
$$0=\frac{dL}{d\phi}-\frac{d}{dt}\frac{dL}{d \dot \phi}$$
$$\frac{dL}{d\phi}=0,\ \ \ \frac{d}{dt}\frac{dL}{d\dot \phi}=\frac{1}{32}mR^2\ddot\phi$$
$$0=0-\frac{1}{32}mR^2\ddot \phi \implies \ddot \phi = 0$$
We repeat wrt to the big disk angle $\theta$ using the above lagrangian and computing partials,
$$0=\frac{dL}{d\theta}-\frac{d}{dt}\frac{dL}{d \dot \theta}$$
$$\frac{dL}{d\theta}=-\frac{mgR}{2}\sin(\theta),\ \ \ \frac{d}{dt}\frac{dL}{d\dot \theta}=(\frac{3}{2}MR^2+\frac{1}{4}mR^2)\ddot\theta$$
Combining, 
$$0=-\frac{mgR}{2}\sin(\theta)-
(\frac{3}{2}MR^2+\frac{1}{4}mR^2)\ddot\theta
\implies \ddot \theta=\frac{-\frac{mgR}{2}\sin(\theta)}{(\frac{3}{2}MR^2+\frac{1}{4}mR^2)}=\ddot \theta=\frac{-mgR\sin(\theta)}{3MR^2+\frac{1}{2}mR^2}$$
Collecting our EOM,
$$
\boxed{\begin{cases}
\ddot \phi = 0\\
\ddot \theta=\frac{-mgR\sin(\theta)}{3MR^2+\frac{1}{2}mR^2}
\end{cases}}
$$
###### Part D
This says that the rotational speed of the little disk $\dot \phi$ is constant. This says that there is symmetry in the energy and momentum in the rotation of the little disk, and that $\fbox{Rotational Energy and Momentum is conserved in the little disk}$. No torques act on the little disk.







