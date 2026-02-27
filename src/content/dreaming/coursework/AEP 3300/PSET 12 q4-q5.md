Bryant Har, bjh254
# Q4

### a
$\boxed{\text{Yes. } \omega_3 \text{ is constant}}$

We have
$$
\begin{cases}
\lambda_1 \dot \omega_1 = (\lambda_2 - \lambda_3)\omega_2 \omega_3\\
\lambda_2 \dot \omega_2 = (\lambda_3 - \lambda_1)\omega_1 \omega_3\\
\lambda_3 \dot \omega_3 = (\lambda_1 - \lambda_2)\omega_1 \omega_2
\end{cases}
$$
If $\lambda_2=\lambda_1$,
$$
\begin{cases}
\lambda_1 \dot \omega_1 = (\lambda_1 - \lambda_3)\omega_1 \omega_3\\
\lambda_1 \dot \omega_2 = (\lambda_3 - \lambda_1)\omega_1 \omega_3=-(\lambda_1 - \lambda_3)\omega_1 \omega_3\\
\lambda_3 \dot \omega_3 = (\lambda_1 - \lambda_1)\omega_1 \omega_1=0
\end{cases}
\implies 
\begin{cases}
\dot \omega_1 = -\dot \omega_2 \\
\lambda_3 \dot \omega_3 =0
\end{cases}
\implies
\dot \omega_3 =0
$$
$$ \dot \omega_3 =0 \implies \omega_3=C,\ \ \  \boxed{\omega_3 \text{ is constant}}$$


### b
$$C=\frac{\lambda_2-\lambda_3}{\lambda_2}\omega_3$$
From above,
$$\begin{cases}
    \lambda_1 \dot \omega_1 = (\lambda_1 - \lambda_3)\omega_1 \omega_3\\
\lambda_1 \dot \omega_2 = (\lambda_3 - \lambda_1)\omega_1 \omega_3=-(\lambda_1 - \lambda_3)\omega_1 \omega_3
\end{cases}=
\begin{cases}
    \lambda_2 \dot \omega_1 = (\lambda_2 - \lambda_3)\omega_2 \omega_3\\
\lambda_2 \dot \omega_2 =-(\lambda_2 - \lambda_3)\omega_1 \omega_3
\end{cases}
$$
$$\implies \begin{cases}
    \dot \omega_1 = \frac{\lambda_2 - \lambda_3}{\lambda_2 }\omega_2 \omega_3\\
\dot \omega_2 =-\frac{\lambda_2 - \lambda_3}{\lambda_2 }\omega_1 \omega_3
\end{cases}
$$

$$\boxed{
\begin{cases}
    \dot \omega_1 = C \omega_2\\
\dot \omega_2 =-C\omega_1
\end{cases}
 \implies \text{Yes}}$$
As desired 


### c
$$\boxed{\dot \eta = -iC\eta}$$
Doing as told,
$$
\begin{cases}
    \dot \omega_1 = C \omega_2\\
\dot \omega_2 =-C\omega_1
\end{cases}
 \implies 
\dot \omega_1 + i\dot \omega_2 = C\omega_2 - Ci\omega_1
$$
Letting,
$$
\eta = \omega_1 + i \omega_2 \implies 
\boxed{\dot \eta = -iC\eta}
$$


### d
$\boxed{\text{Yes}}$
Differentiating, we see that $b=1$ and they are equivalent
$$\frac{d}{dt}\eta = \frac{d}{dt} \eta_0 e^{-iCbt} = 
-iCb\eta_0 e^{-iCbt} =-iCb \eta 
$$
This result is in the same form as above, $\implies \boxed{\text{Yes}}$. $b=1$

### e

$\fbox{Yes}$
Using the given condition,


$$\eta = \omega_0 e^{-iCt} = \omega_1 + i \omega_2 = \omega_0 \big[\cos(-Ct) + i\sin (-Ct)\big]$$
$$\omega_1 + i \omega_2 = \omega_0 \cos(Ct) - i\omega_0 \sin (Ct)$$
Comparing real and imaginary parts in isolation, we find,
$$\boxed{\begin{cases}
    \omega_1  = \omega_0 \cos(Ct)\\
    \omega_2 = - \omega_0 \sin (Ct)
\end{cases} \implies \text{Yes} } $$
### f
$\boxed{|\vec \omega| = \sqrt{\omega_0^2 +\omega_3^2}}$
This would be
$$|\vec \omega| = \sqrt{\omega_1^2+\omega_2^2+\omega_3^2}=
\sqrt{\omega_0^2 \big[\cos(Ct)^2+
(-\sin (Ct))^2\big]
+\omega_3^2}
$$
$$\boxed{|\vec \omega| = 
\sqrt{\omega_0^2 +\omega_3^2}}$$



### g
$\fbox{Yes.}$ 

Yes we would observe precession if $\omega_0 >0$. Therefore, the vector would rotate in a cone.

# Q5

### a
$\fbox{After second rotation}$
The first two rotations set the plane of rotation, while the last rotation sets the body axes to the appropriate orientation within that rotating plane. The $\vec e_1'$ lies on this plane and is rotated into position by the second rotation about $\vec e_2'$. So, the $e_1$ and $e_1'$ vectors point along the same direction after this rotation, or the $\fbox{second rotation}$

### b
By Taylor,
$$\omega = \dot \phi \hat z + \dot \theta e_2' + \dot \psi e_3$$
We seek to convert this into the body unit vectors. We already have $\hat z = \cos(\theta) e_3 - \sin \theta e_1'$,
$$\omega =  \dot \phi\cos(\theta) e_3 - \dot \phi\sin (\theta) e_1' + \dot \theta e_2' + \dot \psi e_3$$
From Taylor's diagram, we see that
$$\begin{cases}
e_1' = \cos \psi e_1 - \sin \psi e_2 \\
e_2' = \cos \psi e_2 + \sin \psi e_1
\end{cases}$$
Substituting,
$$\vec \omega =  \dot \phi\cos(\theta) e_3 - \dot \phi\sin (\theta) (\cos \psi e_1 - \sin \psi e_2) + \dot \theta (\cos \psi e_2 + \sin \psi e_1) + \dot \psi e_3$$
$$\vec \omega =  \dot \phi\cos(\theta) e_3 - \dot \phi\sin (\theta) \cos \psi e_1 + \dot \phi\sin (\theta) \sin \psi e_2 + \dot \theta \cos \psi e_2 + \dot \theta \sin \psi e_1 + \dot \psi e_3$$
$$\boxed{\vec \omega =  \big[\dot \theta \sin \psi - \dot \phi\sin (\theta) \cos \psi \big] e_1 +\big[\dot \theta \cos \psi + \dot \phi\sin (\theta) \sin \psi \big] e_2 + 
\big[ \dot \phi\cos(\theta)+\dot \psi  \big]e_3}$$

$$\boxed{\vec \omega = 
\begin{bmatrix}
\dot \theta \sin \psi - \dot \phi\sin (\theta) \cos \psi \\
\dot \theta \cos \psi + \dot \phi\sin (\theta) \sin \psi\\
\dot \phi\cos(\theta)+\dot \psi
\end{bmatrix}}
$$

### c
$\fbox{1: One DOF}$
$\boxed{\theta}$

Clearly, there is only one degree of freedom, since the cylinder rotates at a constant angular velocity. Only $\theta$ can vary independently here. $\fbox{One degree of freedom}$.


### d
We first find the Lagrangian,
$$\mathcal L = T - V$$
$$V = mgz = -mg\frac{L}{2}\cos\theta$$
$$T = \frac{1}{2}MV^2_{COM} + \frac{1}{2}\omega^T I \omega$$
We find $V_{COM}$ in the inertial frame using 
$$\rho = a + \frac{L}{2}\sin \theta,\ \ \ z = -\frac{L}{2}\cos \theta \ \ \ \dot \phi = \Omega$$
$$V_{COM} = \dot \rho \hat \rho + \rho \dot \phi \hat \phi + \dot z \hat z = 
\dot \theta \frac{L}{2}\cos \theta \hat \rho + (a + \frac{L}{2}\sin \theta) \Omega \hat \phi + \dot \theta \frac{L}{2}\sin \theta  \hat z 
$$
Getting squared magnitude,
$$|V_{COM}|^2 = 
(\dot \theta \frac{L}{2}\cos \theta)^2 + ((a + \frac{L}{2}\sin \theta) \Omega)^2  + (\dot \theta \frac{L}{2}\sin \theta )^2
$$
$$|V_{COM}|^2 = 
\bigg(\frac{L\dot \theta }{2}\bigg)^2+ (a\Omega + \frac{L\Omega}{2}\sin \theta)^2 
$$
Now we find $\omega$ using the above formula, denoting the formula $\theta$ as $\theta'$ and the diagram $\theta$ as $\theta$,
$$ \dot \phi = \Omega, \ \ \ \theta' = \frac{\pi}{2},\ \ \ \psi = \theta$$

$$\vec \omega = 
\begin{bmatrix}
\dot \theta' \sin \psi - \dot \phi\sin (\theta') \cos \psi \\
\dot \theta'
\cos \psi + \dot \phi\sin (\theta'
) \sin \psi\\
\dot \phi\cos(\theta'
)+\dot \psi
\end{bmatrix}=
\begin{bmatrix}
0 \\
0 \\
\Omega + \dot \theta
\end{bmatrix}
$$
Clearly, we only need the $I_{zz}$ term, which is the moment about the COM or, by table,
$$I = \frac{1}{4}MR^2+\frac{1}{12}ML^2$$
Combining our results into $T = \frac{1}{2}MV^2_{COM} + \frac{1}{2}\omega^T I \omega$,
$$T = \frac{1}{2}M\bigg(\bigg(\frac{L\dot \theta }{2}\bigg)^2+ (a\Omega + \frac{L\Omega}{2}\sin \theta)^2\bigg)
 + \frac{1}{2} (\frac{1}{4}Mr^2+\frac{1}{12}ML^2)(\Omega + \dot \theta)^2 $$
 $$T = \frac{1}{2}M\bigg(\frac{L\dot \theta }{2}\bigg)^2+ \frac{1}{2}M(a\Omega + \frac{L\Omega}{2}\sin \theta)^2
 + (\frac{1}{8}Mr^2+\frac{1}{24}ML^2)(\Omega + \dot \theta)^2 $$
Recall,

$$V = -mg\frac{L}{2}\cos\theta$$

$$\mathcal L = T - V$$

We arrive at our Lagrangian,
$$\boxed{\mathcal L = 
\frac{1}{2}M\bigg(\frac{L\dot \theta }{2}\bigg)^2+ \frac{1}{2}M(a\Omega + \frac{L\Omega}{2}\sin \theta)^2
 + (\frac{1}{8}Mr^2+\frac{1}{24}ML^2)(\Omega + \dot \theta)^2
 +Mg\frac{L}{2}\cos\theta
}$$