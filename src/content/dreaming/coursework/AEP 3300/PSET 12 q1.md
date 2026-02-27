#physics 
# Q1
### Q1a
We know that $I_{ij} =m_\alpha (\delta_{ij}|r'_\alpha|^2-x_{\alpha i}x_{\alpha j})$,
Then, the middle row is

$$\boxed{
-m_\alpha\big[ x_{\alpha 2}x_{\alpha 1}\big ] \ \ \ \ \ \
-m_\alpha\big[ x_{\alpha 1}^2+x_{\alpha 3}^2\big ] \ \ \ \ \ \
-m_\alpha\big[ x_{\alpha 2}x_{\alpha 3}\big ]
}$$
### Q1b
In such a case, by symmetry
$$\boxed{I_{11}=I_{22}}$$
Similarly, substituting the above in, we clearly see
$$\boxed{\frac{1}{2}(I_{11}+I_{22})=I_{11}}$$
### Q1c
We begin
$$I_{xx}=\int \int \int dxdydz \ \gamma (y^2+z^2)=\int \int \int dV \ \gamma (y^2+z^2)$$
Recall that from previous question,
$$\frac{1}{2}(I_{xx}+I_{yy})=I_{xx} \implies 
I_{xx}=\frac{1}{2}\int \int \int dV \ \gamma (x^2+y^2+2z^2)
$$
Converting the cartesian variables to cylindrical,
$$
dV=\rho d\rho d\phi dz \implies
I_{xx}=\frac{1}{2}\int \int \int d\rho d\phi dz \ \gamma\rho (\rho^2+2z^2)
$$
$$
\boxed{
I_{xx}=\frac{\gamma}{2}\int_0^{2\pi} d\phi \int_{z_{min}}^{z_{max}}   dz  \int_{\rho_{min}}^{\rho_{max}}   (\rho^2+2z^2)\rho\ d\rho 
}
$$
### Q1d
Since the object has rotational symmetry about the z-axis, we find that
$$\boxed{I_{xy}=0}$$
This is by axial symmetry. $I_{xy}$ is the product of the $x$ and $y$ coordinates summed over all the masses $dm$. Since there is axial symmetry about the $z$ axis, this means that mass is symmetrically distributed across the x and y directions. Then, the object's mass distribution is mirrored about the z axis. Therefore, the product of the $x$ and $y$ for each point of mass is mirrored by a corresponding mass on the other side, and therefore the contribution is canceled out by its corresponding mirrored point mass. Contributions from one side of the z axis will be canceled out by equal contributions on the other side of the axis.
### Q1e
By the above argument, since there is axial symmetry, all the off diagonal elements are zero.
$$I_{ij} \propto \delta_{ij}$$
Further, since $z$ is an axis of symmetry for the object, we observe that $I_{11}=I_{22}$ as above in part b,
We now find the diagonal elements using the result from c,
$$
I_{xx}=I_{yy}=\frac{\gamma}{2}\int_0^{2\pi} d\phi \int_{z_{min}}^{z_{max}}   dz  \int_{\rho_{min}}^{\rho_{max}}   (\rho^2+2z^2)\rho\ d\rho 
$$
By calculator,
$$=\frac{\gamma}{2}\int_0^{2\pi} d\phi \int_{-z_{sc}}^{z_{sc}}   dz  \int_{0}^{
\rho_{sc}\sqrt{1-\frac{z^2}{z_{sc}^2}}
}
(\rho^2+2z^2)\rho\ d\rho 
=\dfrac{4\gamma{\pi}{\rho}_\text{sc}^2z_\text{sc}\left(z_\text{sc}^2+{\rho}_\text{sc}^2\right)}{15}
$$
We solve $I_{zz}$ the same way by calculating this integral using calculator,
$$I_{zz}=\gamma\int_0^{2\pi} d\phi \int_{-z_{sc}}^{z_{sc}}   dz  \int_{0}^{
\rho_{sc}\sqrt{1-\frac{z^2}{z_{sc}^2}}
}
(\rho^2)\rho\ d\rho 
=

\gamma \pi \dfrac{8{\rho}_\text{sc}^4z_\text{sc}}{15}
$$
Our final tensor is,
$$
I = 
\begin{pmatrix}
\dfrac{4\gamma{\pi}{\rho}_\text{sc}^2z_\text{sc}\left(z_\text{sc}^2+{\rho}_\text{sc}^2\right)}{15} &0&0 \\
0&\dfrac{4\gamma{\pi}{\rho}_\text{sc}^2z_\text{sc}\left(z_\text{sc}^2+{\rho}_\text{sc}^2\right)}{15} & 0\\
0&0& \dfrac{\gamma \pi8{\rho}_\text{sc}^4z_\text{sc}}{15}  
\end{pmatrix}

$$
Using $V=\frac{4\pi\gamma}{3}\rho_{sc}^2z_{sc} \implies \gamma = \frac{M}{V}$,
$$
\boxed{
I = 
M\begin{pmatrix}
\dfrac{z_\text{sc}^2+{\rho}_\text{sc}^2}{5} &0&0 \\
0& \dfrac{z_\text{sc}^2+{\rho}_\text{sc}^2}{5} & 0 \\
0&0&\dfrac{2{\rho}_\text{sc}^2}{5}\end{pmatrix}
}
$$
### Q1f
We do the matrix multiplication,
$$\vec{L} = I \vec \omega=

M\begin{bmatrix}
\dfrac{z_\text{sc}^2+{\rho}_\text{sc}^2}{5} &0&0 \\
0& \dfrac{z_\text{sc}^2+{\rho}_\text{sc}^2}{5} & 0 \\
0&0&\dfrac{2{\rho}_\text{sc}^2}{5}\end{bmatrix}
\begin{bmatrix}
0 \\ 0 \\ \omega
\end{bmatrix}
=
\dfrac{2M{\rho}_\text{sc}^2\omega}{5}
$$
$$\boxed{\vec L = \dfrac{2M{\rho}_\text{sc}^2\omega}{5} \hat{e}_z}$$
