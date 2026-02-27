#physics 
# Q1
### Q1a
In steady state for $F=f_0\cos(\omega t + \delta)$, it's known that
$$x_{ss} = 
\begin{cases}
A\cos (\omega t+ \delta)\\
A_{max} \approx \frac{f_0}{2\beta\omega_0}
\end{cases}
$$
At some later time, energy at max amplitude is given by,
$$E=\frac{1}{2}kx_{max}^2=E=\frac{1}{2}kA^2$$
Recalling $2\beta = \frac{b}{m}$ and $F=-bv$, work over one period at $\omega_0$, then, is
$$\Delta E=\int_t^{t+T}F\frac{dx}{dt}dt \approx
\int_t^{t+T}-2m\beta(-A\omega_0\sin(\omega_0 t + \delta))^2dt
=-2{\pi}A^2{\beta}{m\omega_0}
$$
Subbing in these approximate values ($E,\ |\Delta E|$) we recover the definition in Taylor,
$$Q \approx 2\pi \frac{E}{\Delta E_T}\approx 2\pi\frac{\frac{1}{2}kA^2}{2{\pi}A^2{\beta}{\omega_0}}=\frac{k}{2\beta m\omega_0}=\frac{\omega_0}{2\beta}$$
$$\boxed{Q=\frac{\omega_0}{2\beta} \approx 2\pi \frac{E}{\Delta E_T}} $$
### Q1b
We begin,
$$A^2 = \frac{f_0^2}{(\omega_0^2-\omega^2)^2+(2\beta \omega)^2}$$
By difference of squares,
$$ = \frac{f_0^2}{(\omega_0+\omega)^2(\omega_0-\omega)^2+(2\beta \omega)^2}$$
At $\omega \approx \omega_0\pm \beta$,
$$ \approx \frac{f_0^2}{(2\omega_0)^2(\omega_0-\omega)^2+(2\beta \omega)^2}$$
$$ \approx \frac{f_0^2}{(2\omega_0)^2(\beta)^2+(2\beta \omega)^2}$$
$$ A^2\approx \frac{f_0^2}{2(2\beta \omega_0)^2} \ \ \square$$
For comparison, $A^2$ attains a maximum value of
$$A^2_{max} = \frac{f_0^2}{(2\beta \omega_0)^2} \implies \frac{f_0^2}{2(2\beta \omega_0)^2 }\text{ is half maximum}$$
$$\boxed{ A^2\big|_{\omega \approx \omega_0 \pm \beta}=\frac{f_0^2}{2(2\beta \omega_0)^2} = \text{ half maximum}}$$
# Q2
### Q2a
$\fbox{\textbf{iv}}$
Looking from the graph and using the hint, we arrive at this answer. Large amplitude motion in a slightly nonlinear system is a solid analogue for this, and thinking in this case, we can imagine that stepping the frequency up yields a larger effective resonant frequency than in the other direction about the same point.  Therefore, in this analogous system, the same is also true. For duffing oscillators, effective k becomes the sum of k and $kx^2$, so since $\omega_0^2=\frac{k}{m}$, we see that the frequency must be shifted up for higher amplitudes. From the graph, B is the high point, and A is the low point on the frequency response, and so we observe that as $\omega$ is increasing, the amplitude is higher than when the frequency is decreasing.  This observation is represented by choice iv.
### Q2b
We have
$$ x = A\cos (\omega t - \delta)$$
Cubing and applying identity,
$$ x^3 = A^3\cos^3 (\omega t - \delta)= \frac{A^3}{4}\big[\cos(3\omega t-3\delta)  + 3\cos(\omega t - \delta)\big]$$
We arrive at
$$\boxed{x^3 = \frac{A^3}{4}\big[\cos(3\omega t-3\delta)  + 3\cos(\omega t - \delta)\big]}$$
### Q2c
$\fbox{No. }\ \ \  B \sim small^3$. By cubing, every instance of $B$ is multiplied by either $A$ or $B$ some number of times by binomial theorem. Therefore, all terms contributed by this extra $+B\cos(3\omega t + \delta_B)$ term will at maximum be in order of $small^4$, which is less than our desired threshold of $small^3$.
### Q2d
$$m\ddot x+b\dot x + k_1x + k_3x^3$$$$x = A\cos(\omega t - \delta )+B\cos(3\omega t - \delta_B) $$
We take the derivatives
$$\dot x = -3Bw\sin\left(3wt-d\right)-Aw\sin\left(wt-d\right) $$
$$\ddot x = -9Bw^2\cos\left(3wt-d\right)-Aw^2\cos\left(wt-d\right)$$
From before, $x^3$ contributes nothing. $\dot x$ only includes sines. We take everything else independently. The terms including $\cos(3\omega t - \delta_B)$, are then only from $\ddot x, x$ terms:
$$\boxed{-9\omega^2mB\cos(3\omega t - \delta_B)+k_1B\cos(3\omega t - \delta_B)}$$
Where we ignore all the terms including sines and different harmonics,
### Q2e
$$x = A\cos(\omega t - \delta )+B\cos(3\omega t - \delta_B) $$
We simply use the larger frequency, since the faster frequency is a multiple of the larger,
$$\boxed{T = \frac{2\pi}{\omega}}$$
### Q2f
$\fbox{Yes.}$ $9\omega$ and $3\omega$ is also a multiple of  $\omega$.
### Q2g
Use the smallest frequency, or $\frac{\omega}{2}$, we get
$$T = \frac{2\pi}{\omega/2}=\boxed{T=\frac{4\pi}{\omega}}$$
# Q3
### Q3a
$\fbox{Yes, there exists SDIC}$
We have that 
$$\ln(\frac{\Delta\phi}{\Delta\phi_0}) \sim \lambda t$$
Applying equivalent definitions,
$$\ln(\frac{\delta_n}{\delta_0}) \sim \lambda t$$
From the graph, before the absolute difference between systems stabilizes, we observe an approximate slope of 
$$\lambda t = \frac{8-0}{10-0}t \implies \lambda \sim 0.8$$
Since $\lambda>0$,  $\fbox{there exists SDIC}$
### Q3b
$\fbox{Yes.}$ There is SDIC and the graph clearly exhibits chaos.

### Q3c
$\fbox{Yes. It seems to be approaching Feigenbaum's constant.}$
We are given the values of $r$,

$$r_1 = 3, r_2 = 3.4495, r_3 = 3.5441,  
r_4 = 3.5644, r_5 = 3.5688, r_6 = 3.5697
$$
And,
$$\delta_n = \frac{r_{n}-r_{n-1}}{r_{n+1}-r_{n}}$$
We generate the values accordingly,
$$\delta_2 = 4.752, \delta_3 = 4.660, \dots $$
We see that $\delta_3=4.660$. Given that Feigenbaum's constant is $\approx 4.6692$, we observe an error of only
$$\bigg|\frac{4.660-4.6692}{4.6692}\bigg|=0.001970358948 \implies 0.197\%$$
$\fbox{Yes. It does seem to be approaching Feigenbaum's constant.}$
### Q3d
If SDIC and chaos exist, at least one of $\lambda_x>0$  or  $\lambda_y>0$,
$\boxed{\text{At least one of }\lambda_x, \lambda_y \text{ is greater than 0}}$

### Q3e
$\fbox{Yes. It is possible}$

### Q3f
$\fbox{Yes. It is possible}$

# Q4
### Q4a
We begin with definition,
$$\lim_{r \to 0} N(r) \approx \frac{a}{r^D}$$
Taking the log of both sides,
$$\lim_{r \to 0} \ln(N(r)) \approx \ln(\frac{a}{r^D})$$
By log rules,
$$\boxed{\lim_{r \to 0}\ \ln(N) \approx \ln(a)+D\ln(1/r)}$$
$\ln(a)$ is just an offset, so we find that $\ln(N)$ is proportional to $\ln(1/r)$ by proportionality constant $D$. Plotted as a line, $\fbox{D would be the slope}$.
$$\lim_{r\to 0}\ \ln(N) \propto D\ln(1/r)$$
### Q4b
Counting,
$$\boxed{
\begin{cases}
N(1)= 14\\
N(1/2) = 36\\
N(1/4) = 84
\end{cases}}$$
### Q4c
Using the largest/smallest N values,
$$\ln(N(1/4))-\ln(N(1))) \approx D(\ln(4)-\ln(1))$$
Simplifying,
$$\ln(84)-\ln(14) \approx 1.3863D$$
$$\boxed{D = 1.2925}$$
### Q4d
From 12.30,
$$t = t_0, t_0+1,t_0+2, \dots$$
where period was one unit of time. Letting an arbitrary period of time $\tau$ and an arbitrary start time $t_s$, the equivalent more general times are,
$$\boxed{t = t_s, t_s+\tau,t_s+2\tau, \dots}$$
### Q4e
$\fbox{Yes}$
If the properties remain the same across blow-ups, we can define such a value. The statistical properties of the poincare map remain constant, so we can therefore define a fractal dimension as we take the limit of smaller boxes.
**Yes**
### Q4f
Per the book, it is a **strange attractor**.
$$\fbox{ Strange Attractor}$$



