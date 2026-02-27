










### Q36a
We begin,
$$y_{xx}-\frac{1}{c^2}y_{tt}=0,\ \ \ y(x,0)=y_0 \text{ for } -x_0<x<x_0$$
We define green's function as satisfying
$$g_{xx}-\frac{1}{c^2}g_{tt}=0$$
$$c^2g_{xx}=g_{tt}$$
Taking the Fourier transform wrt $x$,
$$-k^2c^2G=G_{tt}$$
This is a well known differential equation (Hooke's Law). The solution is,
$$G = G_0e^{\pm ikct}$$
As usual,  
$$G_0 = \mathcal{F}(g(x|\xi, 0)) = \mathcal{F}(\delta(x-\xi)) = \frac{1}{\sqrt{2\pi}}e^{-ik\xi}$$
$$\implies G = \frac{1}{\sqrt{2\pi}}e^{\pm ik(ct-\xi)}$$
We recover $g$ using the inverse fourier transform (can verify below are equivalent),
$$g = \mathcal{F}^{-1}\bigg(\frac{1}{\sqrt{2\pi}}e^{\pm ik(ct-\xi)}\bigg)=\delta(x\pm(ct-\xi))=\delta(\xi-(x\pm ct))$$
$$g_+ = \delta(\xi-(x+ ct))$$
$$g_- = \delta(\xi-(x- ct))$$
We combine the retarded and advanced greens functions as a linear combination. Using the first initial condition again,
$$y(x,0)=Ay_0(x)+By_0(x)$$
$$y_0(x)_=Ay_0(x)+By_0(x) \implies A+B=1$$
Using our second initial condition $\dot y(0) = 0$, we find:
$$y(x,t)=Ay_0(x-ct)+By_0(x+ct)$$
$$\dot y(x,0)=0=-cA \dot y_0(x)+Bc\dot y_0(x) \implies cB=cA \implies A=B$$
Combining the retarded and advanced greens functions as a linear combination, 
$$A+B=1,\ \ \ A=B \implies A=B=\frac{1}{2}$$
We recover $y$,
$$y(x,t)=\int d\xi\ y_0(\xi)(g_++g_-)$$
$$y(x,t)=\int_{-\infty}^{\infty} d\xi\ y_0(\xi)(A\delta(\xi-(x- ct))+B\delta(\xi-(x+ ct)))$$
$$y(x,t)=\frac{1}{2}y_0(x-ct)+\frac{1}{2}y_0(x+ct)$$
We finally arrive at,
$$\boxed{y(x,t)=\frac{y_0(x-ct)+y_0(x+ct)}{2},\ \ \ t>0}$$
### Q36b
Letting our test function be $y_0=\sin(x)/x$,
$t=0$
![[Pasted image 20231006232138.png|1000]]
$t=2$
![[Pasted image 20231006232151.png|1000]]
$t=8$
![[Pasted image 20231006232233.png]]

The sinc function was plotted for t=0,2,8. Clearly, we see that this consists of the classic sinc spike that splits into two spikes that propagate outward. Animated on Desmos, this behavior is very clear.

### Q36c
For 
$$y_0(x)=\begin{cases}
1,\ \ \ -1<x<1\\
0,\ \ \ \text{otherwise}
\end{cases}$$
We arrive at,
$$\boxed{y(x,t)=y_1+y_2}$$
$$y_1=\begin{cases}
1/2,\ \ \ -1<x-ct<1\\
0,\ \ \ \text{otherwise}
\end{cases}$$
$$y_2=\begin{cases}
1/2,\ \ \ -1<x+ct<1\\
0,\ \ \ \text{otherwise}
\end{cases}$$
Graphically, we see that this is equivalent to the initial wave decomposing into two subwaves of half the initial height that propagate to the left and right respectively. As time increases, the two subwaves get farther apart. (would also look more wave-like as a fourier series)
### Q36d
From a, we consider a linear combination of the retarded and advanced greens functions but this time by initial conditions, we require $v(x)$ due to the zero'd velocity:
$$y(x,t)=Av(x-ct)+Bv(x+ct)$$
Using the first initial condition again,
$$0=Av(x)+Bv(x) \implies A=-B$$
Using our second initial condition $\dot y(0) = v(x)$, we find:
$$y(x,t)=-Bv(x-ct)+Bv(x+ct)$$
We finally arrive at,
$$\dot y(x,0)=v(x)=cB \dot v(x)+Bc\dot v(x)\implies 2cB=1 \implies B=\frac{1}{2c} \implies A = -\frac{1}{2c}$$
$$\boxed{y(x,t)=\frac{v(x+ct)-v(x-ct)}{2c}},\ \ \ t>0$$
Verifying, this obeys $y_0(0)=0,\ \ \ \dot y_0(0)= \dot y(x,0)$.

Graphing $v(x)=sinc(x)$ again as test function,
$t=0$
![[Pasted image 20231006235239.png]]
$t=1$
![[Pasted image 20231006235323.png]]
$t=10$
![[Pasted image 20231006235347.png]]
Graphically, it appears that from nothing, two subwaves emerge and propagate in opposite directions.