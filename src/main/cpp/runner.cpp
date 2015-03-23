#include <iostream>
#include <cstdio>
#include "arma.hpp"
#include "eigen.hpp"
#include "mtl.hpp"
#include "ublas.hpp"

using namespace std
namespace po = boost::program_options;

void runArmaTests(int num_trials, int m, int n, int k, int a, int, b, int c, int d) {

    cout << "Armadillo Simple Test:\t" << simpleTest_Arma(m, n, num_trials) << endl;
    cout << "Armadillo gemm Test:\t" << gemmTest_Arma(m, n, k, num_trials) << endl;
    cout << "Armadillo mulDense Test:\t" << mulDenseTest_Arma(a, b, c, d, num_trials) << endl;

}

void runEigenTests() {

    cout << "Eigen Simple Test:\t" << simpleTest_Eigen(m, n, num_trials) << endl;
    cout << "Eigen gemm Test:\t" << gemmTest_Eigen(m, n, k, num_trials) << endl;
    cout << "Eigen mulDense Test:\t" << mulDenseTest_Eigen(a, b, c, d, num_trials) << endl;

}

void runUBlasTests() {

    cout << "UBlas Simple Test:\t" << simpleTest_UBlas(m, n, num_trials) << endl;
    cout << "UBlas gemm Test:\t" << gemmTest_UBlas(m, n, k, num_trials) << endl;
    cout << "UBlas mulDense Test:\t" << mulDenseTest_UBlas(a, b, c, d, num_trials) << endl;

}

void runMTLTests() {

    cout << "MTL Simple Test:\t" << simpleTest_MTL(m, n, num_trials) << endl;
    cout << "MTL gemm Test:\t" << gemmTest_MTL(m, n, k, num_trials) << endl;
    cout << "MTL mulDense Test:\t" << mulDenseTest_MTL(a, b, c, d, num_trials) << endl;

}

int main(int argv, char *argv[]) {

    int opt;
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("m", po::value<int>(&opt)->default_value(1024), 
            "numRows of matrices in Simple Test, and gemm Test")
        ("n", po::value<int>(&opt)->default_value(1024),
            "numCols of matrices in Simple Test, and gemm Test")
        ("num-trials", po::value<int>(&opt)->default_value(10), "number of trials")
        ("a", po::value<int>(&opt)->default_value(1024),
            "size matrix A in mulDense Test")
        ("b", po::value<int>(&opt)->default_value(512),
            "size matrix B in mulDense Test")
        ("c", po::value<int>(&opt)->default_value(256),
            "size matrix C in mulDense Test")
        ("d", po::value<int>(&opt)->default_value(128),
            "size matrix D in mulDense Test")
    ;

    return 0;
}
