Bryant Har, bjh254
# Q2
### a
The up component is normal to the surface of the ground and must equal the downward force of gravity.
$$\boxed{F_{up}=Mg}$$
### b
For circular motion, we require a centripetal force:
$$\boxed{F_{inward} = m(R-r\cos\theta)\Omega^2}$$

### c
$\boxed{\text{None}}$ (except for gravity)
There is gravity, $Mg$, pointing downward vertically acting on the disk, but we consider contributions from the ground. In cylindrical coordinates,
$$\frac{F_{\phi}}{m} = 2\dot \rho \dot \phi + \rho \ddot \phi$$
$\dot \rho = \ddot \phi = 0$, so there is no contributions from either type of $\phi$ direction force to the $\phi$ direction acceleration.
### d
$\fbox{Yes}$ from the diagram, torque is perpendicular to the page by cross product.
### e
We have the net torque, as they point in opposite directions
$$\tau = r \times F$$
$$\boxed{\tau_{into} = rF_{inward}\sin\theta-rF_{up}\cos\theta}$$
### f
$\fbox{Yes}$, the disk spins as it rolls without slipping and spinning must be opposite sign to capital omega.
### g
$\fbox{Into the page}$ in order to preserve right hand rule with the given 2 and 3 axes.
### h
The 1 and 2 axes rotate, so they have some time dependence. The 3-axis is constant as defined before. We then have a 1 and 2 contribution from $\Omega$
Overall,
$$\boxed{\omega_1 = \Omega\sin(\omega't)\sin\theta ,\ \ \ \omega_2=
\Omega\cos(\omega't)\sin\theta
,\ \ \ 
\omega_3=\Omega \cos \theta - w'
}$$
### i
By table, we know that along 3-axis, the moment of inertia is $\frac{1}{2}mr^2$. Along a diameter axis, it is $\frac{1}{4}mr^2$, corresponding to the 1 and 2 axes. Since the 1,2,3 axes are principal axes clearly by symmetry, the off-diagonal components are zero. Alternatively, instead of looking up in a table, integration could be done to reach the same result.

Drawing from these conclusions, we find that
$$\boxed{I = \begin{bmatrix}
 \frac{1}{4}Mr^2 & 0 & 0 \\
 0 & \frac{1}{4}Mr^2 &0 \\
 0 &0 & \frac{1}{2}Mr^2\\
\end{bmatrix}}$$
Units encoded in tensor of mass x distance squared.
### j
Torque is related to the body frame and inertial frame angular momenta,
$$\Gamma = \dot L + \omega \times L$$
$$\vec\Gamma = \dot {I\omega} + \omega \times (I\omega)$$
$$\vec\Gamma =  {\begin{bmatrix}
 \frac{1}{4}Mr^2 & 0 & 0 \\
 0 & \frac{1}{4}Mr^2 &0 \\
 0 &0 & \frac{1}{2}Mr^2\\
\end{bmatrix} \dot
\omega} + \omega \times (
\begin{bmatrix}
 \frac{1}{4}Mr^2 & 0 & 0 \\
 0 & \frac{1}{4}Mr^2 &0 \\
 0 &0 & \frac{1}{2}Mr^2\\
\end{bmatrix}
\omega)
$$
$$\vec \omega =\begin{bmatrix}
\Omega\sin(\omega't)\sin\theta \\
\Omega\cos(\omega't)\sin\theta \\
\Omega \cos \theta - w'
\end{bmatrix} $$
$$\vec\Gamma = \frac{d}{dt}
\begin{bmatrix} 
\frac{1}{4}Mr^2\Omega\sin(\omega't)\sin\theta \\
\frac{1}{4}Mr^2\Omega\cos(\omega't)\sin\theta \\
\frac{1}{2}Mr^2\big[\Omega \cos \theta - w'\big]
\end{bmatrix}
+ \begin{bmatrix} 
\Omega\sin(\omega't)\sin\theta \\
\Omega\cos(\omega't)\sin\theta \\
\Omega \cos \theta - w'
\end{bmatrix}
\times 
\begin{bmatrix} 
\frac{1}{4}Mr^2\Omega\sin(\omega't)\sin\theta \\
\frac{1}{4}Mr^2\Omega\cos(\omega't)\sin\theta \\
\frac{1}{2}Mr^2\big[\Omega \cos \theta - w'\big]
\end{bmatrix}
$$
$$\vec\Gamma = 
\frac{1}{4}Mr^2\begin{bmatrix} 
\Omega\omega'\cos(\omega't)\sin\theta +(\Omega \cos \theta - w')(\Omega\cos(\omega't)\sin\theta)\\
-\Omega\omega'\sin(\omega't)\sin\theta -(\Omega \cos \theta - w')(\Omega\sin(\omega't)\sin\theta)\\
0
\end{bmatrix}
$$
$$\vec\Gamma = 
\frac{1}{4}Mr^2\Omega(\Omega\omega' +\Omega \cos \theta - w')\begin{bmatrix} 
\cos(\omega't)\\
-\sin(\omega't)\\
0
\end{bmatrix}
\sin\theta
$$
Breaking it up into components
$$\boxed{\begin{cases}
\Gamma_1 = \frac{1}{4}Mr^2\Omega(\Omega\omega' +\Omega \cos \theta - w')\cos(\omega't)\sin\theta\\
\Gamma_2 = -\frac{1}{4}Mr^2\Omega(\Omega\omega' +\Omega \cos \theta - w')\sin(\omega't)\sin\theta\\
\Gamma_3 = 0
\end{cases}
}
$$
As expected, one is proportional to $\cos(\omega' t)$ and another is to $\sin(\omega' t)$
### k
At $t=0$, the 1 axis points into the page.  Using the above expression,
$$\Gamma_1(t) = \frac{1}{4}Mr^2\Omega(\Omega\omega' +\Omega \cos \theta - 
w')\cos(\omega't)\sin\theta$$
$$\boxed{\Gamma_1(t=0) = \frac{1}{4}Mr^2\Omega(\Omega\omega' +\Omega \cos \theta - w')\sin\theta}$$
### l
We know, the speeds must match
$$\frac{2\pi R}{2\pi/\Omega}=\frac{2\pi r}{2\pi/\omega'}$$
$$R\Omega=r\omega' \implies \boxed{\Omega = \frac{r}{R}\omega'}$$
# Q3
### a
$\fbox{Yes}$. Eigenvectors should be perpendicular
### b
We find eigenvectors.
$$\begin{vmatrix}
 5-\lambda & \sqrt{3} & 0 \\
 \sqrt{3} & 3-\lambda &0 \\
 0 &0 & 1-\lambda\\
\end{vmatrix}=
(1-\lambda)((5-\lambda)(3-\lambda)-3)=0$$
$$\implies \lambda = 1, 2,6$$
Plugging them back in,
$\lambda = 1$:
$$\begin{bmatrix}
 4 & \sqrt{3} & 0 \\
 \sqrt{3} & 2 &0 \\
 0 &0 & 0\\
\end{bmatrix} \vec{\omega}=0 \implies \vec\omega = \begin{bmatrix} 0 \\0\\1\end{bmatrix}
$$
$\lambda = 2$:
$$\begin{bmatrix}
 3 & \sqrt{3} & 0 \\
 \sqrt{3} & 1 &0 \\
 0 &0 & -1\\
\end{bmatrix} \vec{\omega}=0 \implies \vec\omega = \frac{1}{2}\begin{bmatrix} 
-1\\ \sqrt{3} \\0\end{bmatrix}
$$

$\lambda = 6$:
$$\begin{bmatrix}
 -1 & \sqrt{3} & 0 \\
 \sqrt{3} & -3 &0 \\
 0 &0 & -5\\
\end{bmatrix} \vec{\omega}=0 \implies \vec\omega = \frac{1}{2}\begin{bmatrix} 
\sqrt{3}\\ 1 \\0\end{bmatrix}
$$

$$\boxed{\text{Principle moments} = 1,2,6} \text{ with mass x dist x dist units}$$
$$(\text{\ with appropriate mass x distance squared units })$$
$$\text{Eigenvectors: \ }\boxed{ \vec{\omega}= \begin{bmatrix} 0 \\0\\1\end{bmatrix},\ \ \ \frac{1}{2}\begin{bmatrix} 
-1\\ \sqrt{3} \\0\end{bmatrix},\ \ \ 
\frac{1}{2}\begin{bmatrix} 
\sqrt{3}\\ 1 \\0\end{bmatrix}
}$$
### c
$\fbox{Out of the page}$ by handedness of cross product and torque
### d
$\fbox{Yes}$, since $L$ points along 3 axis.
$\fbox{Direction}$ of $L$ since the torque is perpendicular to $L$, only direction should be affected
### e
$\fbox{Yes}$
We have
$$\vec{R} \times M \vec g = \frac{d}{dt}\big[ \lambda_3\dot \phi \hat e _3 \big]$$
Moving constants,
$$R \hat e_3 \times M g (-\hat z) = \lambda_3\dot \phi \frac{d \hat e_3}{dt}$$
$$\boxed{\frac{d \hat e_3}{dt}=\frac{1}{\lambda_3\dot \phi}R \hat e_3 \times M g (-\hat z)} \implies \boxed{\text{Yes}}$$
### f
$\fbox{Yes}$. If $\hat e_3$ gives the position vector, and omega is equivalent as above, this is true by substituting values and observing.
### g
$\frac{d \hat e_3}{dt}$ is in the same direction as $\dot \phi$, since it sweeps a circle in its precession movement. So,
$$|\dot \phi| = \bigg|\frac{d \hat e_3}{dt}\bigg|=\frac{MgR}{\lambda_3\dot \psi}|\hat z \times \hat e_3|=|\vec \omega \times \vec r|$$
But,
$$|\vec \omega \times \vec r| = R|\dot \phi||\hat z \times \hat e_3| \implies 
\frac{MgR}{\lambda_3\dot \psi}=|\dot \phi| 
$$
$$\boxed{
|\dot \phi|=\frac{MgR}{\lambda_3\dot \psi}
}
$$







