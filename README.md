linalg-benchmarks
=================

Scripts to run Benchmarks between some linear algebra libraries

Clone this repo to your local machine and run using `bin/run` from the root directory.

Benchmarks prepared to run on Mac OSX.

Turn off multi-threading in your machine from [Instruments](https://developer.apple.com/library/mac/documentation/DeveloperTools/Conceptual/InstrumentsUserGuide/InstrumentsQuickStart/InstrumentsQuickStart.html)
 -> Preferences -> CPUs and set the number of threads to 1.
 
You may set the parameters for the tests in `test_parameters.txt`.

If you already have [Armadillo](http://arma.sourceforge.net/) installed, supply the path to the Armadillo includes
using `ARMA`. If you already have Eigen installed (> Eigen3), you may supply the include folder to Eigen using `EIGEN`.

e.g.

```
ARMA=/path/to/arma EIGEN=path/to/include bin/run
```

Note: Will install homebrew and wget to your machine to download the dependencies

The types of Tests
------------------

### Vector addition

```
e = a + b + c + d
```

Parameters:
 * L: Length of vectors


### Element-wise Matrix Operations

```
      /  A + B      \
X = E | ------- - D |
      \    C        /
```

Parameters:
 * M: Number of rows of all the matrices
 * N: Number of columns of all the matrices

Note: The multiplication is element-wise multiplication

### GEMM Comprehension

```
E += (A + B) * (C - D)
```

Parameters:
 * M: Number of rows of A, B, and E
 * N: Number of columns A and B and number of rows of C and D
 * K: Number of columns of C, D, and E

### Chained GEMM

```
E = W * X * Y * Z
```

Parameters:
 * A: Number of rows of W, and X, number of columns of W
 * B: Number of columns X, and number of rows of Y
 * C: Number of columns Y, number of rows of Z
 * D: Number of columns of Z