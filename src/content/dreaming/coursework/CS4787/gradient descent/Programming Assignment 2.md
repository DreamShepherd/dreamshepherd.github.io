Bryant Har, Eric Sun
### Part 1
The gradient of the loss is derived as follows:
$$R(h_W) = \frac{1}{\mathcal{D}}\sum L(h_W(x), y)+\frac{\gamma}{2}||W||^2$$
$$\nabla_W R(h_W) = \frac{1}{\mathcal{D}}\sum \nabla_WL(h_W(x), y)+\gamma W$$
$$\nabla_W R(h_W) = \frac{1}{\mathcal{D}}\sum_{\mathcal{D}} (\mathrm{softmax}(Wx_i)-y_i)x_i^T+\gamma W$$
The gradient step is then:
$$w_{n+1} = w_n - \alpha \nabla R(h_W)$$
$$w_{n+1} = w_n - \alpha \bigg[ 
\frac{1}{\mathcal{D}}\sum_{\mathcal{D}} (\mathrm{softmax}(Wx_i)-y_i)x_i^T+\gamma W
\bigg]$$
### Parts 2 + 3 Timing Results
##### Non-Vectorized Gradient Descent
Below, for the non-vectorized GD, running 10 iterations took 21.781 seconds. Multiplying by 100, this would take 2,178 seconds or around half an hour for 1000 iterations.

![[Screen Shot 2023-09-29 at 12.27.15 AM.png |500]]
##### Vectorized Gradient Descent
The results using numpy vectorization is shown below. Using numpy optimizations, we observe a 30x speedup. This is significantly faster (21.781s compared to 0.779s). This is due to using C structures and vectorization abstracted away by numpy libraries.

![[Pasted image 20230929011850.png|500]]

### Part 4
Plots of the training and testing error and loss over 1000 iterations are shown below. Loss is given by our loss function, while error is related to the percentage of incorrect predictions. Clearly, across all graphs, we see the typical rapid decrease in loss/error over iterations.

![[Pasted image 20231002113528.png|500]]

### Part 5
Overall, the six methods seem to converge to similar noise balls. By the final iterations, accuracy seems to stabilize at around 89% to 91% correct on the datasets with low variance across the models. Notably, however, the ***runtime*** was significantly different across the variations. Across both batch sizes, sequential stochastic gradient descent consistently ran the fastest, followed by random shuffling without replacement stochastic gradient, and unmodified gradient descent. Random shuffling without replacement did seem to converge slightly faster than normal sgd, which can be seen in the slightly steeper loss graphs. However, the effect was not very strong in this case. There was a dramatic difference in runtimes between batch sizes. For a batch size of 1, runtime over the datasets 24-27 seconds, while for a batch size of 60, that runtime is 9-10 seconds. This is almost a 3x speedup, considering the same number of samples were processed across all models. Further, compared to gradient descent, which took ~75s, this corresponds to a 3x and 10x speedup respectively.

**Note:** *The graphs were plotted against iterations, which offers a finer picture than by epochs. If epochs are desired, you can equivalently consider the plot at every 100 iterations, since there were 10 epochs. Or I can regenerate the plots if requested. All future plots are plotted in epochs as desired.*

![[Pasted image 20231002114317.png|500]]
![[Pasted image 20231002114409.png|500]]
![[Pasted image 20231002114432.png|500]]
![[Pasted image 20231002115054.png|500]]
![[Pasted image 20231002115125.png|500]]
![[Pasted image 20231002115213.png|500]]

### Part 6
##### 1. Using step size other than alpha = 0.05 in Part 5
``` 
alpha = 0.1
batch = 60
gamma = 0.0001
```
Changed alpha from 0.05 to 0.1. Final accuracy was very similar at ~0.91 in 9 seconds. Using 0.05, accuracy was close at around 0.895.
![[Pasted image 20231004191955.png|500]]

##### 2. Improved Accuracy.  
``` 
alpha = 0.1
batch = 25
gamma = 0.0001
```
Of the hyperparameter sets I experimented with, this yielded the best test accuracy (0.92) given how fast it ran (~2-4 seconds). With respect to the training error  (error is $1-\mathrm{accuracy}$), this meant it performed a raw 1-2% better than the given hyperparameters. On the test error, it performed similarly well, yielding an accuracy of around 0.92 and error of 0.08. However, from the graph, you can see some instability around the 8th epoch, when test error slightly jumps before recovering. Test error also decreased.

![[Pasted image 20231004191855.png|500]]

##### 3. Only 5 epochs.
``` 
alpha = 0.3
batch = 60
gamma = 0.0001
```
We aim for around 89-91% accuracy using 5 epochs. Using the same parameters as before but over 5 epochs, we achieve 0.91465 test accuracy over 2 seconds. This is around the same performance as the best runs of SGD, so it is an improvement and it is faster. This corresponds to a test error of about 0.085.
![[Pasted image 20231004192955.png|500]]

##### Plot
The 3 required plots are given in the three parts above. Typically the test error follows the training error closely, which is a good sign.  In general, I only retained the best hyperparameter sets, so all three plots perform either the same or better than the baseline in Part 5. Plots 2 and 3 have close to 0.92 accuracy, which is better than the baseline.

### Part 7
Typical single results are shown below in the plots in the section below. Summarized and averaged over 5 runs, the runtimes are as follows:
***Note that measurements were taken 10x an epoch, slowing overall runtime.***

###### Average Over 5 Runs
- 5.2 - Batch Size = 1
	- SGD Normal: **27 seconds**
	- SGD Sequential: **24 seconds**
	- SGD Random Shuffling: **26 seconds**
- 5.3 - Batch Size = 60
	- SGD Normal: **11.5 seconds**
	- SGD Sequential: **9 seconds**
	- SGD Random Shuffling: **10 seconds**

#### Runtime Analysis
In general, batch size of 60 ran better than batch size of 1. This is because a batch size of 60 leverages numpy vectorization much better than processing each example individually. Processing multiple samples at once significantly speeds up the average processing time per sample. This leads to an 3x speedup.

Between the three types of models, we clearly observe that sequential beats random shuffling without replacement which beats the normal algorithm. This is because the sequential algorithm exploits memory locality to ensure that accesses to each sample are faster across batch iterations, making it the fastest of all. Random shuffling without replacement similarly exploits locality to speed up runtime, but shuffling incurs an overhead penalty that places it behind sequential. It also tends to converge very slightly faster than normal SGD. Finally, normal SGD takes the longest, since it does not use any of these speedup methods and repeatedly generating a new set of random indices at every batch is expensive.


### Various Characteristic Accuracy/Runtime Results (Labeled)
![[Pasted image 20230929162402.png|500]]
![[Pasted image 20231002114743.png|500]]
![[Pasted image 20231002114800.png|500]]
![[Pasted image 20231002114816.png|500]]
![[Pasted image 20230929162500.png|500]]
![[Pasted image 20230929162507.png|500]]
![[Pasted image 20230929162516.png|500]]
