#!/usr/bin/env python3
from tqdm import tqdm
import time
import threading
from matplotlib import pyplot
import pickle
import mnist
import matplotlib
import scipy
from numpy import random
import numpy
import os

# BEGIN THREAD SETTINGS this sets the number of threads used by numpy in the program (should be set to 1 for Parts 1 and 3)
implicit_num_threads = 1
os.environ["OMP_NUM_THREADS"] = str(implicit_num_threads)
os.environ["MKL_NUM_THREADS"] = str(implicit_num_threads)
os.environ["OPENBLAS_NUM_THREADS"] = str(implicit_num_threads)
# END THREAD SETTINGS

matplotlib.use('agg')


mnist_data_directory = os.path.join(os.path.dirname(__file__), "data")

# TODO add any additional imports and global variables


# SOME UTILITY FUNCTIONS that you may find to be useful, from my PA3 implementation
# feel free to use your own implementation instead if you prefer
def multinomial_logreg_error(Xs, Ys, W):
    predictions = numpy.argmax(numpy.dot(W, Xs), axis=0)
    error = numpy.mean(predictions != numpy.argmax(Ys, axis=0))
    return error


def multinomial_logreg_grad_i(Xs, Ys, ii, gamma, W):
    if ii is None:
        batch = Xs
        batch_labels = Ys
        length = len(Xs)
    else:
        batch = Xs[:, ii]
        batch_labels = Ys[:, ii]
        length = len(ii)
    WdotX = numpy.dot(W, batch)
    expWdotX = numpy.exp(WdotX - numpy.amax(WdotX, axis=0))
    softmaxWdotX = expWdotX / numpy.sum(expWdotX, axis=0)
    return numpy.dot(softmaxWdotX - batch_labels, batch.transpose()) / length + gamma * W
# END UTILITY FUNCTIONS


def load_MNIST_dataset():
    PICKLE_FILE = os.path.join(mnist_data_directory, "MNIST.pickle")
    try:
        dataset = pickle.load(open(PICKLE_FILE, 'rb'))
    except:
        # load the MNIST dataset
        mnist_data = mnist.MNIST(mnist_data_directory,
                                 return_type="numpy", gz=True)
        Xs_tr, Lbls_tr = mnist_data.load_training()
        Xs_tr = Xs_tr.transpose() / 255.0
        Ys_tr = numpy.zeros((10, 60000))
        for i in range(60000):
            Ys_tr[Lbls_tr[i], i] = 1.0  # one-hot encode each label
        # shuffle the training data
        numpy.random.seed(4787)
        perm = numpy.random.permutation(60000)
        Xs_tr = numpy.ascontiguousarray(Xs_tr[:, perm])
        Ys_tr = numpy.ascontiguousarray(Ys_tr[:, perm])
        Xs_te, Lbls_te = mnist_data.load_testing()
        Xs_te = Xs_te.transpose() / 255.0
        Ys_te = numpy.zeros((10, 10000))
        for i in range(10000):
            Ys_te[Lbls_te[i], i] = 1.0  # one-hot encode each label
        Xs_te = numpy.ascontiguousarray(Xs_te)
        Ys_te = numpy.ascontiguousarray(Ys_te)
        dataset = (Xs_tr, Ys_tr, Xs_te, Ys_te)
        pickle.dump(dataset, open(PICKLE_FILE, 'wb'))
    return dataset


# SGD + Momentum (adapt from Programming Assignment 3)
#
# Xs              training examples (d * n)
# Ys              training labels   (c * n)
# gamma           L2 regularization constant
# W0              the initial value of the parameters (c * d)
# alpha           step size/learning rate
# beta            momentum hyperparameter
# B               minibatch size
# num_epochs      number of epochs (passes through the training set) to run
#
# returns         the final model arrived at at the end of training
def sgd_mss_with_momentum(Xs, Ys, gamma, W0, alpha, beta, B, num_epochs):
    # TODO students should use their implementation from programming assignment 3
    # or adapt this version, which is from my own solution to programming assignment 3
    (d, n) = Xs.shape
    assert n % B == 0
    W = W0
    V = numpy.zeros(W0.shape)

    for i in range(num_epochs):
        for j in range(n // B):
            ii = range(j*B, (j+1)*B)
            batch = Xs[:, ii]
            batch_labels = Ys[:, ii]

            V = beta * V - alpha * \
                multinomial_logreg_grad_i(
                    batch, batch_labels, None, gamma, W)
            W = W + V

    return W


# SGD + Momentum (No Allocation) => all operations in the inner loop should be a
#   call to a numpy.____ function with the "out=" argument explicitly specified
#   so that no extra allocations occur
#
# Xs              training examples (d * n)
# Ys              training labels   (c * n)
# gamma           L2 regularization constant
# W0              the initial value of the parameters (c * d)
# alpha           step size/learning rate
# beta            momentum hyperparameter
# B               minibatch size
# num_epochs      number of epochs (passes through the training set) to run
# monitor_period  how frequently, in terms of batches (not epochs) to output the parameter vector
#
# returns         the final model arrived at at the end of training
def sgd_mss_with_momentum_noalloc(Xs, Ys, gamma, W0, alpha, beta, B, num_epochs):
    (d, n) = Xs.shape
    (c, d) = W0.shape
    # TODO students should initialize the parameter vector W and pre-allocate any needed arrays here
    V_out = numpy.zeros(W0.shape)
    W_out = W0
    print("Running minibatch sequential-scan SGD with momentum (no allocation)")
    data = [numpy.ascontiguousarray(Xs[:, ibatch*B:(ibatch+1)*B])
            for ibatch in range(int(n/B))]
    labels = [numpy.ascontiguousarray(
        Ys[:, ibatch*B:(ibatch+1)*B]) for ibatch in range(int(n/B))]
    for it in tqdm(range(num_epochs)):
        for i in range(len(data)):
            # TODO this section of code should only use numpy operations with the "out=" argument specified (students should implement this)
            numpy.multiply(beta, V_out, out=V_out)
            numpy.multiply(alpha, multinomial_logreg_grad_i(
                data[i], labels[i], None, gamma, W_out), out=V_out)
            numpy.add(V_out, W_out, out=W_out)

    return W_out


# SGD + Momentum (threaded)
#
# Xs              training examples (d * n)
# Ys              training labels   (c * n)
# gamma           L2 regularization constant
# W0              the initial value of the parameters (c * d)
# alpha           step size/learning rate
# beta            momentum hyperparameter
# B               minibatch size
# num_epochs      number of epochs (passes through the training set) to run
# monitor_period  how frequently, in terms of batches (not epochs) to output the parameter vector
# num_threads     how many threads to use
#
# returns         the final model arrived at at the end of training
def sgd_mss_with_momentum_threaded(Xs, Ys, gamma, W0, alpha, beta, B, num_epochs, num_threads):
    (d, n) = Xs.shape
    (c, d) = W0.shape
    # TODO perform any global setup/initialization/allocation (students should implement this)
    V_out = numpy.zeros(W0.shape)
    W_out = W0
    Bt = int(B/num_threads)
    acc = numpy.zeros((num_threads, c, d))
    # construct the barrier object
    iter_barrier = threading.Barrier(num_threads + 1)

    # a function for each thread to run
    def thread_main(ithread):
        # TODO perform any per-thread allocations
        data = [numpy.ascontiguousarray(
            Xs[:, (ibatch*B + ithread*Bt): (ibatch*B + (ithread+1)*Bt)]) for ibatch in range(int(n/B))]

        labels = [numpy.ascontiguousarray(
            Ys[:, (ibatch*B + ithread*Bt):(ibatch*B + (ithread+1)*Bt)]) for ibatch in range(int(n/B))]
        for it in range(num_epochs):
            for ibatch in range(int(n/B)):
                # TODO work done by thread in each iteration; this section of code should primarily use numpy operations with the "out=" argument specified (students should implement this)
                # ii = range(ibatch*B + ithread*Bt, ibatch*B + (ithread+1)*Bt)
                iter_barrier.wait()
                acc[ithread] = multinomial_logreg_grad_i(
                    data[ibatch], labels[ibatch], None, gamma, W_out)
                iter_barrier.wait()

    worker_threads = [threading.Thread(
        target=thread_main, args=(it,)) for it in range(num_threads)]

    for t in worker_threads:
        print("running thread ", t)
        t.start()

    grad_sum = numpy.zeros((c, d))
    print("Running minibatch sequential-scan SGD with momentum (%d threads)" % num_threads)
    for it in tqdm(range(num_epochs)):
        for ibatch in range(int(n/B)):
            iter_barrier.wait()
            # TODO work done on a single thread at each iteration; this section of code should primarily use numpy operations with the "out=" argument specified (students should implement this)
            numpy.sum(acc, axis=0, out=grad_sum)
            numpy.multiply(alpha/num_threads, grad_sum, out=grad_sum)
            numpy.multiply(beta, V_out, out=V_out)
            numpy.subtract(V_out, grad_sum, out=V_out)
            numpy.add(V_out, W_out, out=W_out)
            iter_barrier.wait()

    for t in worker_threads:
        t.join()

    # return the learned model
    return W_out


# SGD + Momentum (No Allocation) in 32-bits => all operations in the inner loop should be a
#   call to a numpy.____ function with the "out=" argument explicitly specified
#   so that no extra allocations occur
#
# Xs              training examples (d * n)
# Ys              training labels   (c * n)
# gamma           L2 regularization constant
# W0              the initial value of the parameters (c * d)
# alpha           step size/learning rate
# beta            momentum hyperparameter
# B               minibatch size
# num_epochs      number of epochs (passes through the training set) to run
# monitor_period  how frequently, in terms of batches (not epochs) to output the parameter vector
#
# returns         the final model arrived at at the end of training
def sgd_mss_with_momentum_noalloc_float32(Xs, Ys, gamma, W0, alpha, beta, B, num_epochs):
    (d, n) = Xs.shape
    (c, d) = W0.shape
    # TODO students should implement this by copying and adapting their 64-bit code
    V_out = numpy.zeros(W0.shape, dtype=numpy.float32)
    W_out = W0

    data = [numpy.ascontiguousarray(
        Xs[:, ibatch*B:(ibatch+1)*B], dtype=numpy.float32) for ibatch in range(int(n/B))]
    labels = [numpy.ascontiguousarray(
        Ys[:, ibatch*B:(ibatch+1)*B], dtype=numpy.float32) for ibatch in range(int(n/B))]
    print("Running minibatch sequential-scan SGD with momentum (no allocation)")
    for it in tqdm(range(num_epochs)):
        for ibatch in range(int(n/B)):
            # TODO this section of code should only use numpy operations with the "out=" argument specified (students should implement this)
            numpy.multiply(beta, V_out, out=V_out, dtype=numpy.float32)
            numpy.multiply(alpha, multinomial_logreg_grad_i(
                data[ibatch], labels[ibatch], None, gamma, W_out), out=V_out, dtype=numpy.float32)
            numpy.add(V_out, W_out, out=W_out, dtype=numpy.float32)

    return W_out


# SGD + Momentum (threaded, float32)
#
# Xs              training examples (d * n)
# Ys              training labels   (c * n)
# gamma           L2 regularization constant
# W0              the initial value of the parameters (c * d)
# alpha           step size/learning rate
# beta            momentum hyperparameter
# B               minibatch size
# num_epochs      number of epochs (passes through the training set) to run
# monitor_period  how frequently, in terms of batches (not epochs) to output the parameter vector
# num_threads     how many threads to use
#
# returns         the final model arrived at at the end of training
def sgd_mss_with_momentum_threaded_float32(Xs, Ys, gamma, W0, alpha, beta, B, num_epochs, num_threads):
    (d, n) = Xs.shape
    (c, d) = W0.shape
    # TODO students should implement this by copying and adapting their 64-bit code
    V_out = numpy.zeros(W0.shape, dtype=numpy.float32)
    W_out = W0
    Bt = int(B/num_threads)
    acc = numpy.zeros((num_threads, c, d), dtype=numpy.float32)
    # construct the barrier object
    iter_barrier = threading.Barrier(num_threads + 1)

    # a function for each thread to run
    def thread_main(ithread):
        # TODO perform any per-thread allocations
        data = [numpy.ascontiguousarray(Xs[:, ibatch*B + ithread*Bt: ibatch*B + (
            ithread+1)*Bt], dtype=numpy.float32) for ibatch in range(int(n/B))]
        labels = [numpy.ascontiguousarray(Ys[:, ibatch*B + ithread*Bt: ibatch*B + (
            ithread+1)*Bt], dtype=numpy.float32) for ibatch in range(int(n/B))]
        for it in range(num_epochs):
            for ibatch in range(int(n/B)):
                # TODO work done by thread in each iteration; this section of code should primarily use numpy operations with the "out=" argument specified (students should implement this)
                # ii = range(ibatch*B + ithread*Bt, ibatch*B + (ithread+1)*Bt)
                iter_barrier.wait()
                acc[ithread] = multinomial_logreg_grad_i(
                    data[ibatch], labels[ibatch], None, gamma, W_out)
                iter_barrier.wait()

    worker_threads = [threading.Thread(
        target=thread_main, args=(it,)) for it in range(num_threads)]

    for t in worker_threads:
        print("running thread ", t)
        t.start()

    grad_sum = numpy.zeros((c, d))
    print("Running minibatch sequential-scan SGD with momentum (%d threads)" % num_threads)
    for it in tqdm(range(num_epochs)):
        for ibatch in range(int(n/B)):
            iter_barrier.wait()
            # TODO work done on a single thread at each iteration; this section of code should primarily use numpy operations with the "out=" argument specified (students should implement this)
            numpy.sum(acc, axis=0, out=grad_sum, dtype=numpy.float32)
            numpy.multiply(alpha, grad_sum, out=grad_sum, dtype=numpy.float32)
            numpy.multiply(beta, V_out, out=V_out, dtype=numpy.float32)
            numpy.subtract(V_out, grad_sum, out=V_out, dtype=numpy.float32)
            numpy.add(V_out, W_out, out=W_out, dtype=numpy.float32)
            iter_barrier.wait()

    for t in worker_threads:
        t.join()

    # return the learned model
    return W_out


if __name__ == "__main__":
    (Xs_tr, Ys_tr, Xs_te, Ys_te) = load_MNIST_dataset()
    # TODO add code to produce figures
    alpha = 0.1
    beta = 0.9
    B = 16
    gamma = 0.0001
    epochs = 20
    d, n = Xs_tr.shape
    c, n = Ys_tr.shape
    W0 = numpy.zeros((c, d))

    # sgd_mss_with_momentum experiment part 1
    # print("sgd with momentum now")
    # start_time = time.time()
    # model = sgd_mss_with_momentum(
    #     Xs_tr, Ys_tr, gamma, W0, alpha, beta, B, epochs)
    # end_time = time.time()
    # print("Time to run sgd_mss_with_momentum: ", end_time - start_time)

    # # sgd_mss_with_momentum_no_alloc experiment part 1
    # print("sgd with momentum no alloc now")
    # start_time = time.time()
    # model = sgd_mss_with_momentum_noalloc(
    #     Xs_tr, Ys_tr, gamma, W0, alpha, beta, B, epochs)
    # end_time = time.time()
    # print("Time to run sgd_mss_with_momentum_noalloc: ", end_time - start_time)

    batch_sizes = [8, 16, 30, 60, 200, 600, 3000]
    # noalloc_time = []
    alloc_time = []
    # noalloc_time = []
    # alloc_time_manual = []
    for batch_size in batch_sizes:
        print("batch_size: ", batch_size)
        start_time = time.time()
        model = sgd_mss_with_momentum(
            Xs_tr, Ys_tr, gamma, W0, alpha, beta, batch_size, epochs)
        end_time = time.time()
        alloc_time.append(end_time - start_time)

        # start_time = time.time()
        # model = sgd_mss_with_momentum_noalloc(
        #     Xs_tr, Ys_tr, gamma, W0, alpha, beta, batch_size, epochs)
        # end_time = time.time()
        # noalloc_time.append(end_time - start_time)

        # manual threading
        # start_time = time.time()
        # model = sgd_mss_with_momentum_threaded(
        #     Xs_tr, Ys_tr, gamma, W0, alpha, beta, batch_size, epochs, 6)
        # end_time = time.time()
        # alloc_time.append(end_time - start_time)

        # start_time = time.time()
        # model = sgd_mss_with_momentum_noalloc_float32(
        #     Xs_tr, Ys_tr, gamma, W0, alpha, beta, batch_size, epochs)
        # end_time = time.time()
        # alloc_time.append(end_time - start_time)

        # manual threading
        start_time = time.time()
        # model = sgd_mss_with_momentum_threaded_float32(
        #     Xs_tr, Ys_tr, gamma, W0, alpha, beta, batch_size, epochs, 6)
        # end_time = time.time()
        # alloc_time.append(end_time - start_time)

    print("Alloc times: ", alloc_time)
    # print("alloc time namual: ", alloc_time_manual)
    # print("No alloc times: ", noalloc_time)

    alloc_time_onethread = [12.612918853759766, 8.16386103630066, 6.08771824836731,
                            4.881009101867676, 4.100835084915161, 4.02573299407959, 4.37134313583374]
    noalloc_time_onethread = [8.873052835464478, 5.687515020370483, 4.222916841506958,
                              3.3628792762756348, 2.8979883193969727, 2.9398398399353027, 3.2144529819488525]

    alloc_time_multithread = [31.823784828186035, 18.61808681488037, 10.635234117507935,
                              6.137876987457275, 2.946824789047241, 2.1444242000579834, 2.066541910171509]
    noalloc_time_multithread = [22.856566667556763, 14.832613945007324, 7.862540006637573,
                                4.187793970108032, 1.3740007877349854, 0.9208822250366211, 0.8155591487884521]

    noalloc_time_manual = [88.22407412528992, 51.220250844955444, 30.193567037582397,
                           14.529842138290405, 5.202972173690796, 2.994633197784424, 0.98354172706604]

    noalloc_time_onethread_float32 = [10.131762266159058, 6.553134918212891, 4.814266204833984,
                                      3.9413259029388428, 3.2820076942443848, 3.2410972118377686, 3.4897620677948]
    noalloc_time_manual_float32 = [72.50783395767212, 41.67685127258301, 23.865103244781494,
                                   12.569712162017822, 5.603087902069092, 2.5307741165161133, 1.081907033920288]
    noalloc_time_multithread_float32 = [26.709460973739624, 15.032322883605957, 10.590687990188599,
                                        6.055254697799683, 3.493912696838379, 1.7850091457366943, 1.2130811214447021]

    # alloc_time_multithread = alloc_time
    # noalloc_time_multithread = noalloc_time

    # alloc_time_onethread = alloc_time
    # noalloc_time_onethread = noalloc_time

    # noalloc_time_manual = alloc_time

    # pyplot.plot(batch_sizes, noalloc_time_manual,
    #             '#7d3e1b', label='manual multithread without memory allocation, float64')
    # pyplot.plot(batch_sizes, alloc_time_onethread,
    #             '#52700a', label='one thread with memory allocation, float64')
    # pyplot.plot(batch_sizes, noalloc_time_onethread,
    #             '#1cba53', label='one thread without memory allocation, float64')
    # pyplot.plot(batch_sizes, alloc_time_multithread, '#0ee3ca',
    #             label='multithread with memory allocation, float64')
    # pyplot.plot(batch_sizes, noalloc_time_multithread, '#1536d6',
    #             label='multithread without memory allocation, float64')

    # pyplot.plot(batch_sizes, noalloc_time_onethread_float32, '#9f2fd6',
    #             label='one thread without memory allocation, float32')
    # pyplot.plot(batch_sizes, noalloc_time_manual_float32, '#ff75e1',
    #             label='manual multithread without memory allocation, float32')
    # pyplot.plot(batch_sizes, noalloc_time_multithread_float32, '#d4cf3f',
    #             label='multithread without memory allocation, float32')
    # pyplot.title('Time vs. minibatch size part 4')
    # pyplot.xlabel('Minibatch size')
    # pyplot.ylabel('Wall clock time')
    # pyplot.ylim([0, 35])
    # pyplot.legend()
    # pyplot.savefig('part4_graph.png')
    # pyplot.close()
