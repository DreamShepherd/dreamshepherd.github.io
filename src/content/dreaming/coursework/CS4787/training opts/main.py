#!/usr/bin/env python3
import os
import numpy
from numpy import random

# import scipy
import matplotlib
import pickle

# matplotlib.use("agg")
from matplotlib import pyplot as plt

import torch
import torchvision

# you may wish to import other things like nn
from torch.utils.data import DataLoader
import torch.nn as nn

# hyperparameter settings and other constants
batch_size = 100
num_classes = 10
epochs = 10
mnist_input_shape = (28, 28, 1)
d1 = 1024
d2 = 256
alpha = 0.1
beta = 0.9
alpha_adam = 0.001
rho1 = 0.99
rho2 = 0.999
# end hyperparameter settings


# load the MNIST dataset using TensorFlow/Keras
def load_MNIST_dataset():
    train_dataset = torchvision.datasets.MNIST(
        root="./data",
        train=True,
        transform=torchvision.transforms.ToTensor(),
        download=True,
    )
    test_dataset = torchvision.datasets.MNIST(
        root="./data",
        train=False,
        transform=torchvision.transforms.ToTensor(),
        download=False,
    )
    return (train_dataset, test_dataset)


# construct dataloaders for the MNIST dataset
#
# train_dataset        input train dataset (output of load_MNIST_dataset)
# test_dataset         input test dataset (output of load_MNIST_dataset)
# batch_size           batch size for training
# shuffle_train        boolean: whether to shuffle the training dataset
#
# returns              tuple of (train_dataloader, test_dataloader)
#     each component of the tuple should be a torch.utils.data.DataLoader object
#     for the corresponding training set;
#     use the specified batch_size and shuffle_train values for the training DataLoader;
#     use a batch size of 100 and no shuffling for the test data loader


def construct_dataloaders(train_dataset, test_dataset, batch_size, shuffle_train=True):
    train_dataloader = DataLoader(
        train_dataset, batch_size=batch_size, shuffle=shuffle_train
    )
    test_dataloader = DataLoader(test_dataset, batch_size=batch_size, shuffle=False)
    return train_dataloader, test_dataloader


# evaluate a trained model on MNIST data
#
# dataloader    dataloader of examples to evaluate on
# model         trained PyTorch model
# loss_fn       loss function (e.g. nn.CrossEntropyLoss)
#
# returns       tuple of (loss, accuracy), both python floats
@torch.no_grad()
def evaluate_model(dataloader, model, loss_fn):
    model.eval()
    loss = correct = total = 0
    for inputs, labels in dataloader:
        output = model(inputs)
        loss += len(labels) * loss_fn(output, labels).item()
        correct += sum(torch.argmax(output, dim=1) == labels)
        total += output.shape[0]
    avg_loss = loss / total
    acc = correct / total
    return avg_loss, acc


def make_fully_connected_model_part1_1():
    return nn.Sequential(
        nn.Flatten(),
        nn.LazyLinear(out_features=1024),
        nn.ReLU(),
        nn.Linear(in_features=1024, out_features=256),
        nn.ReLU(),
        nn.Linear(in_features=256, out_features=10),
    )

    # build a fully connected two-hidden-layer neural network with Batch Norm, as in Part 1.4
    # use the default initialization for the parameters provided in PyTorch
    # returns   a new model of type nn.Sequential


def make_fully_connected_model_part1_4():
    return nn.Sequential(
        nn.Flatten(),
        nn.LazyLinear(out_features=1024),
        nn.BatchNorm1d(1024),
        nn.ReLU(),
        nn.Linear(in_features=1024, out_features=256),
        nn.BatchNorm1d(256),
        nn.ReLU(),
        nn.Linear(in_features=256, out_features=10),
    )


# build a convolutional neural network, as in Part 3.1
# use the default initialization for the parameters provided in PyTorch
#
# returns   a new model of type nn.Sequential


def make_cnn_model_part3_1():
    return nn.Sequential(
        nn.LazyConv2d(16, (3, 3), 1, 0),
        nn.LazyBatchNorm2d(),
        nn.ReLU(),
        nn.LazyConv2d(16, (3, 3), 1, 0),
        nn.LazyBatchNorm2d(),
        nn.ReLU(),
        nn.MaxPool2d((2, 2)),
        nn.LazyConv2d(32, (3, 3), 1, 0),
        nn.LazyBatchNorm2d(),
        nn.ReLU(),
        nn.MaxPool2d((2, 2)),
        nn.Flatten(),
        nn.LazyLinear(out_features=128),
        nn.ReLU(),
        nn.Linear(128, 10),
    )

    # train a neural network on MNIST data
    #     be sure to call model.train() before training and model.eval() before evaluating!
    #
    # train_dataloader   training dataloader
    # test_dataloader    test dataloader
    # model              dnn model to be trained (training should mutate this)
    # loss_fn            loss function
    # optimizer          an optimizer that inherits from torch.optim.Optimizer
    # epochs             number of epochs to run
    # eval_train_stats   boolean; whether to evaluate statistics on training set each epoch
    # eval_test_stats    boolean; whether to evaluate statistics on test set each epoch
    #
    # returns   a tuple of
    #   train_loss       an array of length `epochs` containing the training loss after each epoch, or [] if eval_train_stats == False
    #   train_acc        an array of length `epochs` containing the training accuracy after each epoch, or [] if eval_train_stats == False
    #   test_loss        an array of length `epochs` containing the test loss after each epoch, or [] if eval_test_stats == False
    #   test_acc         an array of length `epochs` containing the test accuracy after each epoch, or [] if eval_test_stats == False
    #   approx_tr_loss   an array of length `epochs` containing the average training loss of examples processed in this epoch
    #   approx_tr_acc    an array of length `epochs` containing the average training accuracy of examples processed in this epoch


def train(
    train_dataloader,
    test_dataloader,
    model,
    loss_fn,
    optimizer,
    epochs,
    eval_train_stats=True,
    eval_test_stats=True,
):
    train_loss, train_acc, test_loss, test_acc, approx_tr_loss, approx_tr_acc = (
        [],
        [],
        [],
        [],
        [],
        [],
    )
    for _ in range(epochs):
        model.train()
        total_loss = total_correct = total_examples = 0
        for batch_input, batch_labels in train_dataloader:
            optimizer.zero_grad()
            preds = model(batch_input)
            loss = loss_fn(preds, batch_labels)
            loss.backward()
            optimizer.step()
            total_loss += len(batch_labels) * loss.item()
            total_correct += sum(torch.argmax(preds, dim=1) == batch_labels)
            total_examples += len(batch_labels)
        approx_tr_loss.append(total_loss / total_examples)
        approx_tr_acc.append(total_correct / total_examples)
        if eval_train_stats:
            loss, acc = evaluate_model(train_dataloader, model, loss_fn)
            train_loss.append(loss)
            train_acc.append(acc)
        if eval_test_stats:
            loss, acc = evaluate_model(test_dataloader, model, loss_fn)
            test_loss.append(loss)
            test_acc.append(acc)
    return (train_loss, train_acc, test_loss, test_acc, approx_tr_loss, approx_tr_acc)


if __name__ == "__main__":
    (train_dataset, test_dataset) = load_MNIST_dataset()
    train_dataloader, test_dataloader = construct_dataloaders(
        train_dataset, test_dataset, 100
    )

    # # SGD

    # model_1_1 = make_fully_connected_model_part1_1()
    # cross_entropy = nn.CrossEntropyLoss()
    # SGD_opt = torch.optim.SGD(model_1_1.parameters(), lr=0.1)
    # train_loss, train_acc, test_loss, test_acc, approx_tr_loss, approx_tr_acc = train(
    #     train_dataloader, test_dataloader, model_1_1, cross_entropy, SGD_opt, 10
    # )
    # xaxis = range(epochs)
    # plt.plot(xaxis, train_loss, label="End of Epoch Training Loss")
    # plt.plot(xaxis, test_loss, label="Test Loss")
    # plt.plot(xaxis, approx_tr_loss, label="Minibatch Average Training Loss")
    # plt.title("SGD Loss")
    # plt.ylabel("Loss")
    # plt.xlabel("Epochs")
    # plt.legend()
    # plt.show()
    # plt.plot(xaxis, train_acc, label="End of Epoch Training Acc")
    # plt.plot(xaxis, test_acc, label="Test Acc")
    # plt.plot(xaxis, approx_tr_acc, label="Minibatch Average Training Acc")
    # plt.title("SGD Accuracy")
    # plt.ylabel("Accuracy")
    # plt.xlabel("Epochs")
    # plt.legend()
    # plt.show()

    # # SGD with Momentum

    # model_1_2 = make_fully_connected_model_part1_1()
    # cross_entropy = nn.CrossEntropyLoss()
    # momentum_opt = torch.optim.SGD(model_1_2.parameters(), lr=0.1, momentum=0.9)
    # train_loss, train_acc, test_loss, test_acc, approx_tr_loss, approx_tr_acc = train(
    #     train_dataloader, test_dataloader, model_1_2, cross_entropy, momentum_opt, 10
    # )
    # xaxis = range(epochs)
    # plt.plot(xaxis, train_loss, label="End of Epoch Training Loss")
    # plt.plot(xaxis, test_loss, label="Test Loss")
    # plt.plot(xaxis, approx_tr_loss, label="Minibatch Average Training Loss")
    # plt.title("Momentum SGD Loss")
    # plt.ylabel("Loss")
    # plt.xlabel("Epochs")
    # plt.legend()
    # plt.show()
    # plt.plot(xaxis, train_acc, label="End of EpochTraining Acc")
    # plt.plot(xaxis, test_acc, label="Test Acc")
    # plt.plot(xaxis, approx_tr_acc, label="Minibatch Average Training Acc")
    # plt.title("Momentum SGD Accuracy")
    # plt.ylabel("Accuracy")
    # plt.xlabel("Epochs")
    # plt.legend()
    # plt.show()

    # # Adam

    # model_1_3 = make_fully_connected_model_part1_1()
    # cross_entropy = nn.CrossEntropyLoss()
    # adam_opt = torch.optim.Adam(model_1_3.parameters(), lr=0.001, betas=(0.99, 0.999))
    # train_loss, train_acc, test_loss, test_acc, approx_tr_loss, approx_tr_acc = train(
    #     train_dataloader, test_dataloader, model_1_3, cross_entropy, adam_opt, 10
    # )
    # xaxis = range(epochs)
    # plt.plot(xaxis, train_loss, label="End of Epoch Training Loss")
    # plt.plot(xaxis, test_loss, label="Test Loss")
    # plt.plot(xaxis, approx_tr_loss, label="Minibatch Average Training Loss")
    # plt.title("Adam Loss")
    # plt.ylabel("Loss")
    # plt.xlabel("Epochs")
    # plt.legend()
    # plt.show()
    # plt.plot(xaxis, train_acc, label="End of Epoch Training Acc")
    # plt.plot(xaxis, test_acc, label="Test Acc")
    # plt.plot(xaxis, approx_tr_acc, label="Minibatch Average Training Acc")
    # plt.title("Adam Accuracy")
    # plt.ylabel("Accuracy")
    # plt.xlabel("Epochs")
    # plt.legend()
    # plt.show()

    # # Batch Normalization with Momentum

    # model_1_4 = make_fully_connected_model_part1_4()
    # cross_entropy = nn.CrossEntropyLoss()
    # momentum_opt = torch.optim.SGD(model_1_4.parameters(), lr=0.001, momentum=0.9)
    # train_loss, train_acc, test_loss, test_acc, approx_tr_loss, approx_tr_acc = train(
    #     train_dataloader, test_dataloader, model_1_4, cross_entropy, momentum_opt, 10
    # )
    # xaxis = range(epochs)
    # plt.plot(xaxis, train_loss, label="End of Epoch Training Loss")
    # plt.plot(xaxis, test_loss, label="Test Loss")
    # plt.plot(xaxis, approx_tr_loss, label="Minibatch Average Training Loss")
    # plt.title("Batch Normalized Momentum SGD Loss")
    # plt.ylabel("Loss")
    # plt.xlabel("Epochs")
    # plt.legend()
    # plt.show()
    # plt.plot(xaxis, train_acc, label="End of Epoch Training Acc")
    # plt.plot(xaxis, test_acc, label="Test Acc")
    # plt.plot(xaxis, approx_tr_acc, label="Minibatch Average Training Acc")
    # plt.title("Batch Normalized Momentum SGD Accuracy")
    # plt.ylabel("Accuracy")
    # plt.xlabel("Epochs")
    # plt.legend()
    # plt.show()

    # ##########

    # # Convolutional Adam

    # model_3_1 = make_cnn_model_part3_1()
    # cross_entropy = nn.CrossEntropyLoss()
    # adam_opt = torch.optim.Adam(model_3_1.parameters(), lr=0.001, betas=(0.99, 0.999))
    # train_loss, train_acc, test_loss, test_acc, approx_tr_loss, approx_tr_acc = train(
    #     train_dataloader,
    #     test_dataloader,
    #     model_3_1,
    #     cross_entropy,
    #     adam_opt,
    #     10,
    #     False,
    #     False,
    # )
    # xaxis = range(epochs)
    # # plt.plot(xaxis, train_loss, label="End of Epoch Training Loss")
    # plt.plot(xaxis, test_loss, label="Test Loss")
    # plt.plot(xaxis, approx_tr_loss, label="Minibatch Average Training Loss")
    # plt.title("Convolutional Adam Loss")
    # plt.ylabel("Loss")
    # plt.xlabel("Epochs")
    # plt.legend()
    # plt.show()
    # # plt.plot(xaxis, train_acc, label="Training Acc")
    # plt.plot(xaxis, test_acc, label="Test Acc")
    # plt.plot(xaxis, approx_tr_acc, label="Minibatch Average Training Acc")
    # plt.title("Convolutional Adam Accuracy")
    # plt.ylabel("Accuracy")
    # plt.xlabel("Epochs")
    # plt.legend()
    # plt.show()
