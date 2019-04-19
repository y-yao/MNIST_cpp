# MNIST_cpp

A simple handwriting recognition system written in C++ using machine learning algorithms (brute force KNN and KNN with a k-dimensional tree) leveraging several programming paradigms (procedural, object-oriented, and generic).

## To compile and run
```
git clone https://github.com/y-yao/MNIST_cpp
cd MNIST_cpp
mkdir build_eval
cd build_eval
cmake .. -DCMAKE_BUILD_TYPE=RELEASE
make eval
```
An evaluation report of the prediction accuracy will be produced.
