#include <iostream>
#include <cstdio>
#include <ctime>
#include <Eigen/Dense>
#include "boost/program_options.hpp"

using namespace std;
using namespace Eigen;
namespace po = boost::program_options;

// m: numRows, n: numCols
inline double simpleDenseTest_Eigen(const int m, const int n, const int num_trials) {
  MatrixXd A = MatrixXd::Random(m, n);
  MatrixXd B = MatrixXd::Random(m, n);
  MatrixXd C = MatrixXd::Random(m, n);
  MatrixXd D = MatrixXd::Random(m, n);
  MatrixXd E = MatrixXd::Random(m, n);
  
  clock_t start;
  double duration = 0.0;
    
  for (unsigned i = 0; i < num_trials; i++) {
    start = clock();

    MatrixXd res = ((A + B).cwiseQuotient(C) - D).cwiseProduct(E);

    duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  }

  return duration / num_trials;

}

// m: numRows of A, n: numCols of A, and numRows of B, k: numCols of B
inline double gemmDenseTest_Eigen(const int m, const int n, const int k, const int num_trials) {

  MatrixXd A = MatrixXd::Random(m, n);
  MatrixXd B = MatrixXd::Random(m, n);
  MatrixXd C = MatrixXd::Random(n, k);
  MatrixXd D = MatrixXd::Random(n, k);
  MatrixXd E = MatrixXd::Random(m, k);

  clock_t start;
  double duration = 0.0;
    
  for (unsigned i = 0; i < num_trials; i++) {
    start = clock();

    E.noalias() += (A + B) * (C - D);

    duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  }

  return duration / num_trials;

}

inline double mulDenseTest_Eigen(const int a, const int b, const int c, const int d, const int num_trials) {

  MatrixXd A = MatrixXd::Random(a, a);
  MatrixXd B = MatrixXd::Random(a, b);
  MatrixXd C = MatrixXd::Random(b, c);
  MatrixXd D = MatrixXd::Random(c, d);

  clock_t start;
  double duration = 0.0;
    
  for (unsigned i = 0; i < num_trials; i++) {
    start = clock();

    MatrixXd res = A * B * C * D;

    duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  }

  return duration / num_trials;

}

void runEigenTests(int num_trials, int m, int n, int k, int a, int b, int c, int d) {

   cout << "Eigen Simple Test:\t" << simpleDenseTest_Eigen(m, n, num_trials) << endl;
   cout << "Eigen gemm Test:\t" << gemmDenseTest_Eigen(m, n, k, num_trials) << endl;
   cout << "Eigen mulDense Test:\t" << mulDenseTest_Eigen(a, b, c, d, num_trials) << endl;

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

    runEigenTests(trials, m, n, k, a, b, c, d);

    return 0;
}
