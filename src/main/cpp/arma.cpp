#include <iostream>
#include <cstdio>
#include <ctime>
#include <armadillo>
#include "boost/program_options.hpp"

using namespace std;
using namespace arma;
namespace po = boost::program_options;

// m: numRows, n: numCols
inline double simpleDenseTest_Arma(int m, int n, int num_trials) {

  mat A = randu<mat>(m, n);
  mat B = randu<mat>(m, n);
  mat C = randu<mat>(m, n);
  mat D = randu<mat>(m, n);
  mat E = randu<mat>(m, n);

  clock_t start;
  double duration = 0.0;

  for (unsigned i = 0; i < num_trials; i++) {
    start = clock();

    mat res = ((A + B) / C - D) % E;

    duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  }
  return duration / num_trials;

}

// m: numRows of A, n: numCols of A, and numRows of B, k: numCols of B
inline double gemmDenseTest_Arma(int m, int n, int k, int num_trials) {

  mat A = randu<mat>(m, n);
  mat B = randu<mat>(m, n);
  mat C = randu<mat>(n, k);
  mat D = randu<mat>(n, k);
  mat E = randu<mat>(m, k);

  clock_t start;
  double duration = 0.0;
  
  for (unsigned i = 0; i < num_trials; i++) {
    start = clock();

    E += (A + B) * (C - D);

    duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  }
  
  return duration / num_trials;

}

inline double mulDenseTest_Arma(int a, int b, int c, int d, int num_trials) {

  mat A = randu<mat>(a, a);
  mat B = randu<mat>(a, b);
  mat C = randu<mat>(b, c);
  mat D = randu<mat>(c, d);

  clock_t start;
  double duration = 0.0;
    
  for (unsigned i = 0; i < num_trials; i++) {
    start = clock();

    mat res = A * B * C * D;

    duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  }
  return duration / num_trials;
}

void runArmaTests(int num_trials, int m, int n, int k, int a, int b, int c, int d) {

  cout << "Armadillo Simple Test:\t" << simpleDenseTest_Arma(m, n, num_trials) << endl;
  cout << "Armadillo gemm Test:\t" << gemmDenseTest_Arma(m, n, k, num_trials) << endl;
  cout << "Armadillo mulDense Test:\t" << mulDenseTest_Arma(a, b, c, d, num_trials) << endl;

}

int main(int argc, char *argv[]) {

    int m;
    int n;
    int trials;
    int a;
    int b;
    int c;
    int d;
    int k;
    
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("m", po::value<int>(&m)->default_value(1024),
            "numRows of matrices in Simple Test, and gemm Test")
        ("n", po::value<int>(&n)->default_value(1024),
            "numCols of matrices in Simple Test, and gemm Test")
        ("k", po::value<int>(&k)->default_value(1024),
            "numCols of B in gemm Test")
        ("trials", po::value<int>(&trials)->default_value(10), "number of trials")
        ("a", po::value<int>(&a)->default_value(1024),
            "size matrix A in mulDense Test")
        ("b", po::value<int>(&b)->default_value(512),
            "size matrix B in mulDense Test")
        ("c", po::value<int>(&c)->default_value(256),
            "size matrix C in mulDense Test")
        ("d", po::value<int>(&d)->default_value(128),
            "size matrix D in mulDense Test")
    ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    runArmaTests(trials, m, n, k, a, b, c, d);

    return 0;
}