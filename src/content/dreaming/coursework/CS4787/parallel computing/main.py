#!/usr/bin/env python3
import os

# BEGIN THREAD SETTINGS this sets the number of threads used by numpy in the program (should be set to 1 for Parts 1 and 3)
implicit_num_threads = 4

os.environ["OMP_NUM_THREADS"] = str(implicit_num_threads)
os.environ["MKL_NUM_THREADS"] = str(implicit_num_threads)
os.environ["OPENBLAS_NUM_THREADS"] = str(implicit_num_threads)
# END THREAD SETTINGS

import numpy
from numpy import random
import scipy
import matplotlib
import mnist
import pickle

matplotlib.use("agg")
from matplotlib import pyplot
import threading
import time

from tqdm import tqdm

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
        ii = range(batch.shape[1])
    else:
        batch = Xs[:, ii]
        batch_labels = Ys[:, ii]

    WdotX = numpy.dot(W, batch)
    expWdotX = numpy.exp(WdotX - numpy.amax(WdotX, axis=0))
    softmaxWdotX = expWdotX / numpy.sum(expWdotX, axis=0)
    return (
        numpy.dot(softmaxWdotX - batch_labels, batch.transpose()) / len(ii) + gamma * W
    )


# END UTILITY FUNCTIONS


def load_MNIST_dataset():
    PICKLE_FILE = os.path.join(mnist_data_directory, "MNIST.pickle")
    try:
        dataset = pickle.load(open(PICKLE_FILE, "rb"))
    except:
        # load the MNIST dataset
        mnist_data = mnist.MNIST(mnist_data_directory, return_type="numpy", gz=True)
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
        pickle.dump(dataset, open(PICKLE_FILE, "wb"))
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
    models = []
    (d, n) = Xs.shape
    V = numpy.zeros(W0.shape)
    W = W0
    print("Running minibatch sequential-scan SGD with momentum")
    for it in tqdm(range(num_epochs)):
        for ibatch in range(int(n / B)):
            ii = range(ibatch * B, (ibatch + 1) * B)
            V = beta * V - alpha * multinomial_logreg_grad_i(Xs, Ys, ii, gamma, W)
            W = W + V
    return W
    #         if ((ibatch+1) % monitor_period == 0):
    #             models.append(W)
    # return models


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
    V = numpy.zeros(W0.shape)
    W = W0
    Xs_presliced = [
        numpy.ascontiguousarray(Xs[:, ibatch * B : (ibatch + 1) * B])
        for ibatch in range(int(n / B))
    ]
    Ys_presliced = [
        numpy.ascontiguousarray(Ys[:, ibatch * B : (ibatch + 1) * B])
        for ibatch in range(int(n / B))
    ]
    print("Running minibatch sequential-scan SGD with momentum (no allocation)")
    for it in tqdm(range(num_epochs)):
        for ibatch in range(int(n / B)):
            # TODO this section of code should only use numpy operations with the "out=" argument specified (students should implement this)
            numpy.multiply(beta, V, out=V)
            numpy.multiply(
                alpha,
                multinomial_logreg_grad_i(
                    Xs_presliced[ibatch],
                    Ys_presliced[ibatch],
                    None,
                    gamma,
                    W,
                ),
                out=V,
            )
            numpy.add(V, W, out=W)
    return W


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
def sgd_mss_with_momentum_threaded(
    Xs, Ys, gamma, W0, alpha, beta, B, num_epochs, num_threads
):
    (d, n) = Xs.shape
    (c, d) = W0.shape
    # TODO perform any global setup/initialization/allocation (students should implement this)
    V = numpy.zeros(W0.shape)
    W = W0
    accumulator = numpy.zeros((num_threads, c, d))
    grad = numpy.zeros((c, d))
    Bt = int(B / num_threads)

    # construct the barrier object
    iter_barrier = threading.Barrier(num_threads + 1)

    # a function for each thread to run
    def thread_main(ithread):
        # TODO perform any per-thread allocations

        Xs_presliced = [
            numpy.ascontiguousarray(
                Xs[:, (ibatch * B + ithread * Bt) : (ibatch * B + (ithread + 1) * Bt)]
            )
            for ibatch in range(int(n / B))
        ]

        Ys_presliced = [
            numpy.ascontiguousarray(
                Ys[:, (ibatch * B + ithread * Bt) : (ibatch * B + (ithread + 1) * Bt)]
            )
            for ibatch in range(int(n / B))
        ]

        for it in range(num_epochs):
            for ibatch in range(int(n / B)):
                # TODO work done by thread in each iteration; this section of code should primarily use numpy operations with the "out=" argument specified (students should implement this)
                # ii = range(ibatch*B + ithread*Bt, ibatch*B + (ithread+1)*Bt)
                iter_barrier.wait()
                accumulator[ithread] = multinomial_logreg_grad_i(
                    Xs_presliced[ibatch], Ys_presliced[ibatch], None, gamma, W
                )
                iter_barrier.wait()

    worker_threads = [
        threading.Thread(target=thread_main, args=(it,)) for it in range(num_threads)
    ]

    for t in worker_threads:
        print("running thread ", t)
        t.start()

    print(
        "Running minibatch sequential-scan SGD with momentum (%d threads)" % num_threads
    )
    for it in tqdm(range(num_epochs)):
        for ibatch in range(int(n / B)):
            iter_barrier.wait()
            # TODO work done on a single thread at each iteration; this section of code should primarily use numpy operations with the "out=" argument specified (students should implement this)
            numpy.sum(accumulator, axis=0, out=grad)
            numpy.multiply(alpha / num_threads, grad, out=grad)
            numpy.multiply(beta, V, out=V)
            numpy.subtract(V, grad, out=V)
            numpy.add(V, W, out=W)
            iter_barrier.wait()

    for t in worker_threads:
        t.join()

    # return the learned model
    return W


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
def sgd_mss_with_momentum_noalloc_float32(
    Xs, Ys, gamma, W0, alpha, beta, B, num_epochs
):
    (d, n) = Xs.shape
    (c, d) = W0.shape
    # TODO students should implement this by copying and adapting their 64-bit code

    V = numpy.zeros(W0.shape)
    W = W0
    Xs_presliced = [
        numpy.ascontiguousarray(Xs[:, ibatch * B : (ibatch + 1) * B])
        for ibatch in range(int(n / B))
    ]
    Ys_presliced = [
        numpy.ascontiguousarray(Ys[:, ibatch * B : (ibatch + 1) * B])
        for ibatch in range(int(n / B))
    ]
    print("Running minibatch sequential-scan SGD with momentum (no allocation)")
    for it in tqdm(range(num_epochs)):
        for ibatch in range(int(n / B)):
            # TODO this section of code should only use numpy operations with the "out=" argument specified (students should implement this)
            numpy.multiply(beta, V, out=V, dtype=numpy.float32)
            numpy.multiply(
                alpha,
                multinomial_logreg_grad_i(
                    Xs_presliced[ibatch], Ys_presliced[ibatch], None, gamma, W
                ),
                out=V,
                dtype=numpy.float32,
            )
            numpy.add(V, W, out=W, dtype=numpy.float32)

    return W


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
def sgd_mss_with_momentum_threaded_float32(
    Xs, Ys, gamma, W0, alpha, beta, B, num_epochs, num_threads
):
    (d, n) = Xs.shape
    (c, d) = W0.shape
    # TODO students should implement this by copying and adapting their 64-bit code
    V = numpy.zeros(W0.shape, dtype=numpy.float32)
    W = W0
    accumulator = numpy.zeros((num_threads, c, d), dtype=numpy.float32)
    grad = numpy.zeros((c, d))
    Bt = int(B / num_threads)

    # construct the barrier object
    iter_barrier = threading.Barrier(num_threads + 1)

    # a function for each thread to run
    def thread_main(ithread):
        # TODO perform any per-thread allocations

        Xs_presliced = [
            numpy.ascontiguousarray(
                Xs[:, (ibatch * B + ithread * Bt) : (ibatch * B + (ithread + 1) * Bt)],
                dtype=numpy.float32,
            )
            for ibatch in range(int(n / B))
        ]

        Ys_presliced = [
            numpy.ascontiguousarray(
                Ys[:, (ibatch * B + ithread * Bt) : (ibatch * B + (ithread + 1) * Bt)],
                dtype=numpy.float32,
            )
            for ibatch in range(int(n / B))
        ]

        for it in range(num_epochs):
            for ibatch in range(int(n / B)):
                # TODO work done by thread in each iteration; this section of code should primarily use numpy operations with the "out=" argument specified (students should implement this)
                # ii = range(ibatch*B + ithread*Bt, ibatch*B + (ithread+1)*Bt)
                iter_barrier.wait()
                accumulator[ithread] = multinomial_logreg_grad_i(
                    Xs_presliced[ibatch], Ys_presliced[ibatch], None, gamma, W
                )
                iter_barrier.wait()

    worker_threads = [
        threading.Thread(target=thread_main, args=(it,)) for it in range(num_threads)
    ]

    for t in worker_threads:
        print("running thread ", t)
        t.start()

    print(
        "Running minibatch sequential-scan SGD with momentum (%d threads)" % num_threads
    )
    for it in tqdm(range(num_epochs)):
        for ibatch in range(int(n / B)):
            iter_barrier.wait()
            # TODO work done on a single thread at each iteration; this section of code should primarily use numpy operations with the "out=" argument specified (students should implement this)
            numpy.sum(accumulator, axis=0, out=grad, dtype=numpy.float32)
            numpy.multiply(alpha / num_threads, grad, out=grad, dtype=numpy.float32)
            numpy.multiply(beta, V, out=V, dtype=numpy.float32)
            numpy.subtract(V, grad, out=V, dtype=numpy.float32)
            numpy.add(V, W, out=W, dtype=numpy.float32)
            iter_barrier.wait()

    for t in worker_threads:
        t.join()

    # return the learned model
    return W


if __name__ == "__main__":
    (Xs_tr, Ys_tr, Xs_te, Ys_te) = load_MNIST_dataset()
    # TODO add code to produce figures
    # graphing done on a separate jupyter notebook,
    # since it's easier to preserve state there instead of recomputing every graph...
    batch_sizes = [8, 16, 30, 60, 200, 600, 3000]

    alloc_time = [
        7.0739099979400635,
        5.126156806945801,
        4.104083776473999,
        3.505143642425537,
        2.9673779010772705,
        2.8968610763549805,
        3.5706679821014404,
    ]
    noalloc_time = [
        5.348340034484863,
        3.9374639987945557,
        2.777128219604492,
        1.967005729675293,
        1.7846379280090332,
        1.823612928390503,
        1.9162461757659912,
    ]

    alloc_time_multithread = [
        6.998680830001831,
        6.8719000816345215,
        3.975306987762451,
        4.3805992603302,
        2.7663516998291016,
        3.211153984069824,
        3.0380749702453613,
    ]
    noalloc_time_multithread = [
        5.592694997787476,
        3.7269370555877686,
        3.5443363189697266,
        3.394838571548462,
        1.542586088180542,
        1.4007618427276611,
        1.1250410079956055,
    ]

    noalloc_time_manual = [
        45.61742687225342,
        24.394543886184692,
        13.322428226470947,
        6.858793020248413,
        2.523045301437378,
        1.1000370979309082,
        0.8316080570220947,
    ]

    alloc_time_32 = [
        6.383808851242065,
        3.9482967853546143,
        2.9007580280303955,
        2.0151331424713135,
        1.927886724472046,
        1.7353417873382568,
        1.7977900505065918,
    ]
    manual_noalloc_time_32 = [
        84.23634910583496,
        41.92745089530945,
        24.81030511856079,
        13.42278790473938,
        2.976414918899536,
        1.9920320510864258,
        1.8075447082519531,
    ]
    multi_noalloc_time_32 = [
        6.4468910694122314,
        4.513223886489868,
        3.6224141120910645,
        2.565131902694702,
        1.3369488716125488,
        1.229236125946045,
        1.229506015777588,
    ]

    pyplot.plot(
        batch_sizes,
        alloc_time,
        "#527cca",
        # label="One Thread, With Memory Allocation, float64",
    )
    pyplot.plot(
        batch_sizes,
        noalloc_time,
        "#1cbaaa",
        # label="One Thread, Without Memory Allocation, float64",
    )
    pyplot.plot(
        batch_sizes,
        alloc_time_multithread,
        "#a40000",
        # label="Multithread With Memory Allocation, float64",
    )
    pyplot.plot(
        batch_sizes,
        noalloc_time_multithread,
        "#fa8072",
        # label="Multithread Without Memory Allocation, float64",
    )
    pyplot.plot(
        batch_sizes,
        noalloc_time_manual,
        "#cc00cc",
        # label="Manual Multithread Without Memory Allocation, float64",
    )

    pyplot.plot(
        batch_sizes,
        alloc_time_32,
        "#8db600",
        # label="One Thread, With Memory Allocation, float32",
    )
    pyplot.plot(
        batch_sizes,
        manual_noalloc_time_32,
        "#008000",
        # label="Manual Multithread Without Memory Allocation, float32",
    )
    pyplot.plot(
        batch_sizes,
        multi_noalloc_time_32,
        "#66ff00",
        # label="Multithread Without Memory Allocation, float32",
    )

    pyplot.title("Time vs Minibatch Size (Close Up)")
    pyplot.xlabel("Minibatch Size")
    pyplot.ylabel("Wall clock time")
    pyplot.ylim([0.8, 2])
    pyplot.legend()
    pyplot.savefig("part4_graph_blownup.png")
    pyplot.close()
