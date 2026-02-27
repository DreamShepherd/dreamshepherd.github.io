


# Q28
$$\int_{-1}^{1} dx\  P'_{\ell}(x)P'_{\ell'}(x) = \frac{2}{2\ell+1}\delta_{\ell \ell'}$$
##### Part a
$$f(x) = \sum a_\ell P'_\ell (x)$$
For some $i$, by orthogonality,
$$\implies \int_{-1}^{1} dx\  P_{i}(x)f(x) = \frac{2a_{i}}{2i+1} \implies a_i = \frac{2i+1}{2}\int_{-1}^{1} dx\  P_{i}(x)f(x)$$
$$a_i = \frac{2i+1}{2}\bigg[\int_{-1}^{0} dx\  P_{i}(x)(x+1)+\int_{0}^{1} dx\  P_{i}(x)(1-x)\bigg]$$
We solve for $i=0,1,2$, using desmos
$$a_0 = \frac{1}{2}\bigg[\int_{-1}^{0} dx\  (x+1)+\int_{0}^{1} dx\  (1-x)\bigg]=0.5$$
$$a_1 = \frac{3}{2}\bigg[\int_{-1}^{0} dx\  x(x+1)+\int_{0}^{1} dx\  x(1-x)\bigg]=0$$
$$a_2 = \frac{5}{2}\bigg[\int_{-1}^{0} dx\  \frac{3x^2-1}{2}(x+1)+\int_{0}^{1} dx\  \frac{3x^2-1}{2}(1-x)\bigg]=-0.625$$
We arrive at $\boxed{a_0=0.5,\ \ \ a_1=0,\ \ \  a_2=-0.625}$
##### Part b
$\boxed{\text{Blows up to\ } +\infty\text{\ or\  }-\infty \text{\ at a polynomial rate}}$
For first 3 terms, blows up to $-\infty$. For fourth, blows up to $+\infty$.

The sign of the infinity depends on the parity of the last nonzero term's exponent, as that term will dominate as $|x| \to \infty$.

##### Part c
We repeat the process, but now the $x$ term has a 1/2 in front
$$a_0 = \frac{1}{2}\bigg[\int_{-1}^{0} dx\  (\frac{1}{2}x+1)+\int_{0}^{1} dx\  (1-\frac{1}{2}x)\bigg]=0.75$$
$$a_1 = \frac{3}{2}\bigg[\int_{-1}^{0} dx\  x(\frac{1}{2}x+1)+\int_{0}^{1} dx\  x(1-\frac{1}{2}x)\bigg]=0$$
$$a_2 = \frac{5}{2}\bigg[\int_{-1}^{0} dx\  \frac{3x^2-1}{2}(\frac{1}{2}x+1)+\int_{0}^{1} dx\  \frac{3x^2-1}{2}(1-\frac{1}{2}x)\bigg]=−0.3125$$
Our constructed series is then
$\boxed{a_0 = 0.75,\ \ \ a_1 = 0, \ \ \ a_2=-0.3125}$
$$\boxed{0.75-0.3125\left(\frac{3x^{2}-1}{2}\right)}$$
##### Part d
Repeating the process, we get
$$a_0 = \frac{1}{2}\bigg[\int_{-1}^{1} dx\  (1-x^2)\bigg]=\frac{2}{3}$$
$$a_1 = \frac{3}{2}\bigg[\int_{-1}^{0} dx\  x(1-x^2)\bigg]=0$$
$$a_2 = \frac{5}{2}\bigg[\int_{-1}^{0} dx\  \frac{3x^2-1}{2}(1-x^2)\bigg]=-\frac{2}{3}$$
$$\boxed{a_0 = 2/3,\ \ \  a_1=0, \ \ \ a_2 = -2/3}$$
**This is identical to the original function, which is to be expected.**
