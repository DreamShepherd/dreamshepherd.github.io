#physics 
# Q1
### Q1a
We begin with
$$\bigg(\frac{dQ}{dt}\bigg)_{IN}=\dot Q_x e_x+Q_x\omega \times e_x + \dot Q_ye_y + Q_y \omega \times e_y + \dot Q_z e_z + Q_z \omega \times e_z$$
Grouping,
$$=\bigg[\dot Q_x e_x+\dot Q_ye_y +\dot Q_z e_z\bigg]+\bigg[Q_x\omega \times e_x + Q_y \omega \times e_y + Q_z \omega \times e_z\bigg]$$
$$\boxed{\bigg(\frac{dQ}{dt}\bigg)_{IN}=\bigg(\frac{dQ}{dt}\bigg)_{ROT}+\vec{\omega} \times \vec{Q} }$$
### Q1b
We begin,
$$\bigg(\frac{dr}{dt}\bigg)_{IN}=\bigg(\frac{dR}{dt}\bigg)_{IN}+v_r+\omega \times r'$$
Taking time derivatives,
$$\bigg(\frac{d^2r}{dt^2}\bigg)_{IN}=\bigg(\frac{d^2R}{dt^2}\bigg)_{IN}+\frac{d}{dt}\bigg(\frac{dr'}{dt}\bigg)_{ROT}+\dot \omega \times r'+
\omega \times \frac{dr'}{dt}$$
Applying Q rule and product rule,
$$\bigg(\frac{d^2r}{dt^2}\bigg)_{IN}=\bigg(\frac{d^2R}{dt^2}\bigg)_{IN}+\bigg(\frac{d^2r'}{dt^2}\bigg)_{ROT}+\omega \times \bigg(\frac{dr'}{dt}\bigg)_{ROT}+\dot \omega \times r'+
\omega \times (\bigg(\frac{dr'}{dt}\bigg)_{ROT}+\omega \times r')$$
$$\bigg(\frac{d^2r}{dt^2}\bigg)_{IN}=\bigg(\frac{d^2R}{dt^2}\bigg)_{IN}+\bigg(\frac{d^2r'}{dt^2}\bigg)_{ROT}+\omega \times v'_r+\dot \omega \times r'+
\omega \times (v'_r+\omega \times r')$$
$$\boxed{\vec{\bigg(\frac{d^2r}{dt^2}\bigg)_{IN}}=\vec{\bigg(\frac{d^2R}{dt^2}\bigg)_{IN}}+\vec{\bigg(\frac{d^2r'}{dt^2}\bigg)_{ROT}}+ \vec{\dot \omega} \times \vec{r'}+\vec{\omega} \times (\vec{\omega} \times \vec{r'})+
2\vec{\omega} \times \vec{v'_r}}$$
As desired

### Q1c
By inspection, angular velocity is constant and rotation frame velocity is zero.
$$\boxed{\vec{\dot \omega} \times \vec{r'} \to 0}$$
$$\boxed{
2\vec{\omega} \times \vec{v'_r}
\to 0}$$
### Q1d
We make the appropriate substitutions,
$$\vec{\bigg(\frac{d^2r}{dt^2}\bigg)_{IN}}=\vec{\bigg(\frac{d^2R}{dt^2}\bigg)_{IN}}+\vec{\bigg(\frac{d^2r'}{dt^2}\bigg)_{ROT}}+ \vec{\dot \omega} \times \vec{r'}+\vec{\omega} \times (\vec{\omega} \times \vec{r'})+
2\vec{\omega} \times \vec{v'_r}$$
$$\vec{\bigg(\frac{d^2r}{dt^2}\bigg)_{IN}}=\vec{g_0}+\vec{\bigg(\frac{d^2r'}{dt^2}\bigg)_{ROT}}+ \vec{\omega} \times (\vec{\omega} \times \vec{r'})$$
Moving centrifugal terms to left side, and multiplying by m
$$F_{tot}-\vec{\omega} \times (\vec{\omega} \times \vec{r'})=m\vec{\bigg(\frac{d^2r'}{dt^2}\bigg)_{ROT}}+m\vec{g_0}$$
We now define $g$
$$\boxed{\vec{g}=\vec{g_0}
-\vec{\omega} \times (\vec{\omega} \times \vec{r'})}$$
We now define $F_{tot}$,
$$F_{moon}+m\vec{g}=m\vec{\bigg(\frac{d^2r'}{dt^2}\bigg)_{ROT}}+m\vec{g_0} $$
$$m\vec{\bigg(\frac{d^2r'}{dt^2}\bigg)_{ROT}}=m\vec{g}+F_{moon}-m\vec{g_0}$$
$$\boxed{F_{tidal} = F_{moon}-m\vec{g_0}}$$
If absolute generality is desired, recall that $g_0 = \vec{\bigg(\frac{d^2R}{dt^2}\bigg)_{IN}}$. Then, more generally, we can say
$$\boxed{F_{tidal} = F_{moon}-m\vec{\bigg(\frac{d^2R}{dt^2}\bigg)_{IN}}
}$$
### Q1e
This is just gravitational force,
$$m\vec{\bigg(\frac{d^2R}{dt^2}\bigg)_{IN}}=-\frac{GM_mm}{r^2}\hat{R}_{M-E}$$
$$\boxed{\vec{\bigg(\frac{d^2R}{dt^2}\bigg)_{IN}}=-\frac{GM_m}{|\vec{R}_{M-E}|^3}\vec{R}_{M-E}}$$
We define it negative, since acceleration of the Earth points from Earth to the Moon. 
### Q1f
We begin,
$$F_{tidal} = F_{moon}-m\vec{\bigg(\frac{d^2R}{dt^2}\bigg)_{IN}}$$
Use the above $\vec{\bigg(\frac{d^2R}{dt^2}\bigg)_{IN}}$ from e, noting that acceleration now points in the same direction as $-R_{M-E}$,
$$F_{tidal} = \bigg[\frac{GM_mm}{(|\vec{R}_{M-E}|+r_E)^2}
+\frac{GM_mm}{|\vec{R}_{M-E}|^3}\vec{R}_{M-E}
\bigg](-\hat{R}_{M-E})$$
$$F_{tidal} = \bigg[\frac{GM_mm}{|\vec{R}_{M-E}|^2(1+\frac{r_E}{|\vec{R}_{M-E}|})^2}
+
\frac{GM_mm}{|\vec{R}_{M-E}|^2}

\bigg](-\hat{R}_{M-E})\approx

\frac{GM_mm}{R_{M-E}^2}\bigg[\frac{
\frac{2r_E}{R_{M-E}}
}{1+\frac{2r_E}{R_{M-E}}}

\bigg](-\hat{R}_{M-E})

$$
$$\boxed{F_{tidal}\approx

\frac{GM_mm}{R_{M-E}^2}\bigg[\frac{
2r_E
}{R_{M-E}+2r_E}

\bigg](-\hat{R}_{M-E})

}$$
$$\fbox{Direction is from Moon to Earth.}$$
$$\boxed{|F_{tidal}| \approx \frac{GM_mm}{R_{M-E}^2}\bigg[\frac{
2r_E
}{R_{M-E}+2r_E}

\bigg]}$$
# Q2
### Q2a
$$\bigg(\frac{dQ}{dt}\bigg)\bigg|_{IN}=\bigg(\frac{dQ}{dt}\bigg)_{ROT}+\bigg[\vec{\omega} \times \vec{Q} \bigg]$$
Applying this, 
$$\bigg(\frac{dr'_\alpha}{dt}\bigg)\bigg|_{IN}=\bigg(\frac{dr'_\alpha}{dt}\bigg)_{ROT}+\vec{\omega} \times \vec{r'_\alpha}$$
Since the masses are glued to the axes directly, the position vectors do not change in the rotating frame. Therefore, 
$$\bigg(\frac{dr'_\alpha}{dt}\bigg)_{ROT} = 0 \implies \boxed{\bigg(\frac{dr'_\alpha}{dt}\bigg)_{FIX}=\vec{\omega} \times \vec{r'_\alpha}}$$
as desired

### Q2b
Multiply both sides by $m_\alpha\vec{r'_\alpha}$,
$$\bigg(\frac{dr'_\alpha}{dt}\bigg)_{FIX}=\vec{\omega} \times \vec{r'_\alpha}\implies m_\alpha\vec{r'_\alpha} \times \bigg(\frac{dr'_\alpha}
{dt}\bigg)_{FIX}=m_\alpha\vec{r'_\alpha} \times (\vec{\omega} \times \vec{r'_\alpha})$$
Applying vector identity,
$$m_\alpha\vec{r'_\alpha} \times \bigg(\frac{dr'_\alpha}{dt}\bigg)_{FIX}=m_\alpha|{r'_\alpha}|^2\vec{\omega}-m_\alpha(\vec{r'_\alpha}\cdot\vec \omega) \vec{r'_\alpha}$$
Sum over all $\alpha$ and use tensor notation,
$$\vec{r'_\alpha} \times m_\alpha\dot r'_\alpha= m_\alpha\bigg[\omega_i|{r'_\alpha}|^2-
x_{\alpha i}(x_{\alpha j}\omega_j)
\bigg]$$Explicitly writing the summations,
$$\boxed{\sum_\alpha \vec{r'_\alpha} \times m_\alpha\dot r'_\alpha=\sum_\alpha m_\alpha\bigg[\omega_i|{r'_\alpha}|^2-
x_{\alpha i}\sum_j x_{\alpha j}\omega_j
\bigg]}$$
As desired
### Q2c
$$\sum_\alpha \vec{r'_\alpha} \times m_\alpha\dot r'_\alpha=\sum_\alpha m_\alpha\bigg[\omega_i|{r'_\alpha}|^2-
x_{\alpha i}\sum_j x_{\alpha j}\omega_j
\bigg]$$
$$I_{ij}=\sum_\alpha m_\alpha \bigg[\delta_{ij}|\vec{r}_\alpha'|^2-x_{\alpha i}x_{\alpha j}\bigg]$$
Rearranging the first, to fit $I_{ij}$,
$$\boxed{
\sum_{j}I_{ij}\omega_j=
\sum_\alpha \vec{r'_\alpha} \times m_\alpha\dot r'_\alpha}
$$
This is just the sum of angular momenta is the moment of inertia multiplied by the angular frequency.
# Q3
### Q3a
$\fbox{West side is further from center}$. Since Earth spins West to East, in northern hemisphere, water deflects west.
### Q3b
The vertical component of angular velocity is $\omega \sin 30=\omega/2$, so the horizontal force is then
$$a_{horiz}=2S \times \omega = 2S\omega/2=S\omega$$
The vertical component is simply gravity, so the bouyant force angle is then below. This is perpendicular to the surface tilt, so finding surface tilt angle inverts the arguments,
$$\tan\theta = \frac{g}{\omega S} \implies \tan(\theta_{tilt})=\frac{\omega S}{g}$$
Height is then
$$\Delta h = W\tan(\theta_{tilt})=\boxed{\Delta h=\frac{S\omega }{g}W}$$
### Q3c
$\fbox{Wind travels west}$.
At the equator, $\fbox{air travels West}$ relative to the ground. It moves from east to west.
This is intuitive from the coriolis effect. Below the equator, air approaches from the South and is therefore deflected to the left by the coriolis effect. This leftward deflection approaching from South corresponds to moving west from East to West. Above the equator in the northern hemisphere, air approaches from the North, and the deflection is toward the right. Air approaches from the north, and again, this corresponds to moving west from East to West.
### Q3d
$\fbox{Larger angular velocity}$. 
Barely above ground so ground travels at around same speed stationary. But also traveling eastward (with the Earth) relative to the ground, so it has a faster angular velocity.
$\fbox{Yes}$ They both point radially outward if the object is moving Eastward at the equator like the rocket.
$\fbox{Yes}$ Simple angular velocity to translational velocity conversion.


# Q4
### Q4a
The angular velocity vector points up from the north pole. Using conventional handedness, the $y'$ direction must be pointing north. From the $\lambda$ latitude, we can draw a diagram and see
$$\boxed{\omega_{x'} = 0, \ \ \ \omega_{y'} = \omega\cos(\lambda),\ \ \ \omega_{z'}=\omega\sin(\lambda)}$$
### Q4b
$$V_{x'} = V_0\cos(\alpha),\ \ \ V_{z'} = V_0\sin(\alpha)$$
From simple physics,
$$\boxed{v_{x'}^{0th}=V_0\cos(\alpha),\ \ \ v_{z'}^{0th}=V_0\sin(\alpha)-gt,\ \ \ v_{y'}^{0th}=0}$$
### Q4c
We have 
$$\omega_{y'} = \omega\cos(\lambda),\ \ \ \omega_{z'}=\omega\sin(\lambda)
$$
Computing $a=2v_r^{0th} \times \omega$ is then trivial,
$$a_{x'}=2\omega (v_{y'}\sin(\lambda) - v_{z'}\cos(\lambda))$$
$$a_{y'}=-2\omega v_{x'}\sin(\lambda),\ \ \ \\ \ a_{z'}=2\omega v_{x'}\cos(\lambda)
$$
We add the zeroth order velocities from the previous answer,
$$\boxed{a_{x'}=-2\omega (V_0\sin(\alpha)-gt)\cos(\lambda)}$$
$$\boxed{a_{y'}=-2\omega V_0\cos(\alpha)\sin(\lambda),\ \ \ \\ \ a_{z'}=2\omega V_0\cos(\alpha)\cos(\lambda)}
$$
### Q4d
We have
$$\ddot z(t)=\vec{g}_z-a_{z'}=-g+2\omega V_0\cos(\alpha)\cos(\lambda)$$
$$\int_0^t dt \ddot z = \dot z(t)-\dot z(0)=(-g+2\omega V_0\cos(\alpha)\cos(\lambda))t,\ \ \ z'(0)=V_0\sin(\alpha)$$
$$\implies \dot z(t)=(2\omega V_0\cos(\alpha)\cos(\lambda)-g)t + V_0\sin(\alpha)$$
Repeating,
$$\int_0^t dt\  \dot z = z(t)-z(0)=(2\omega V_0\cos(\alpha)\cos(\lambda)-g)t^2/2 + V_0\sin(\alpha)t,\ \ \ z(0)=0$$
$$\implies 
\boxed{z'(t)=\frac{2\omega V_0\cos(\alpha)\cos(\lambda)-g}{2}t^2 + V_0\sin(\alpha)t}$$
### Q4e
We set $z(t)=0$ and assume $t>0$,
$$
0=\frac{2\omega V_0\cos(\alpha)\cos(\lambda)-g}{2}t^2 + V_0\sin(\alpha)t$$
$$t=\frac{2V_0\sin(\alpha)}{g-2\omega V_0\cos(\alpha)\cos(\lambda)}$$
We apply the "tiny" approximatin,
$$t=\frac{2V_0\sin(\alpha)}{g}(1-\frac{2\omega V_0\cos(\alpha)\cos(\lambda)}{g})^{-1}$$
We expect $\omega$ to be small, so
$$t\approx\frac{2V_0\sin(\alpha)}{g}(1+\frac{2\omega V_0\cos(\alpha)\cos(\lambda)}{g})$$
$$\boxed{
t\approx\frac{2V_0\sin(\alpha)}{g^2}(g+2\omega V_0\cos(\alpha)\cos(\lambda))
}$$
### Q4f
$\fbox{y'(t) points North}$.
We begin as before,
$$\ddot y(t)=a_{y'}=-2\omega V_0\cos(\alpha)\sin(\lambda)$$
Integrating twice,
$$y'(t)=-\omega V_0\cos(\alpha)\sin(\lambda)t^2+At+B$$
$\dot y(0) = y(0)= 0$, so constants of integration are zero
$$\boxed{y'(t)=-\omega V_0\cos(\alpha)\sin(\lambda)t^2}$$
### Q4f
$\fbox{Yes}$ it is already proportional to $\omega$.

### Q4h
The conditions are met, and north/south correspond to $y$ and $z$. From before, we use the velocity to find position and solve for landing time in zeroth order,
$$v_{z'}^{0th}=V_0\sin(\alpha)-gt \implies z'^{0th}=V_0\sin(\alpha)t-gt^2/2$$
$$\implies z'(0) = 0 \implies t_{landing}= \frac{2V_0}{g}\sin(\alpha )$$
We now use our coriolis-inclusive $y'$ to get,
$$y'(t)=-\omega V_0\cos(\alpha)\sin(\lambda)t^2=-\omega V_0\cos(\alpha)(\frac{2V_0}{g}\sin(\alpha ))^2$$
$$\boxed{y'(t_{landing})=-\omega \frac{4V_0^3}{g^2}\sin^2(\alpha)\cos(\alpha)}$$
The projectile will land $\fbox{South}$ at a distance of $\omega \frac{4V_0^3}{g^2}\sin^2(\alpha)\cos(\alpha)$.

