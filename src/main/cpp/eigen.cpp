#include <iostream>
#include <cstdio>
#include <ctime>

#define EIGEN_NO_DEBUG // turn off assertions

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
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    MatrixXd res = ((A + B).cwiseQuotient(C) - D).cwiseProduct(E);

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  return duration / num_trials;

}

// m: numRows of A, n: numCols of A, and numRows of B, k: numCols of B
inline double gemmSanityTest_Eigen(const int m, const int n, const int k, const int num_trials) {

  MatrixXd A = MatrixXd::Random(m, n);
  MatrixXd C = MatrixXd::Random(n, k);
  MatrixXd E = MatrixXd::Random(m, k);

  clock_t start;
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    E.noalias() += A * C;

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
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
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    E.noalias() += (A + B) * (C - D);

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  return duration / num_trials;

}

inline double mulDenseTest_Eigen(const int a, const int b, const int c, const int d, const int num_trials) {

  MatrixXd A = MatrixXd::Random(a, a);
  MatrixXd B = MatrixXd::Random(a, b);
  MatrixXd C = MatrixXd::Random(b, c);
  MatrixXd D = MatrixXd::Random(c, d);

  clock_t start;
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    MatrixXd res = A * B * C * D;

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  return duration / num_trials;

}

// m: numRows of A, n: numCols of A, and numRows of B, k: numCols of B
inline double denseVectorTest_Eigen(int l, int num_trials) {

  VectorXd a = VectorXd::Random(l);
  VectorXd b = VectorXd::Random(l);
  VectorXd c = VectorXd::Random(l);
  VectorXd d = VectorXd::Random(l);
  VectorXd res(l);

  clock_t start;
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    res = a + b + c + d;

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

  return duration / num_trials;
}

void runEigenTests(int num_trials, int l, int m, int n, int k, int a, int b, int c, int d,
    bool skip_vec, bool skip_simple, bool skip_gemm, bool skip_mult) {
   
  if (!skip_vec)
    cout << "Eigen Vectors Test:\t" << denseVectorTest_Eigen(l, num_trials) << endl;
  if (!skip_simple)
    cout << "Eigen Simple Test:\t" << simpleDenseTest_Eigen(m, n, num_trials) << endl;
  if (!skip_gemm) {
    cout << "Eigen gemmSanity Test:\t" << gemmDenseTest_Eigen(m, n, k, num_trials) << endl;
    cout << "Eigen gemm Test:\t" << gemmDenseTest_Eigen(m, n, k, num_trials) << endl;
  }

  if (!skip_mult)
    cout << "Eigen mulDense Test:\t" << mulDenseTest_Eigen(a, b, c, d, num_trials) << endl;

}

int main(int argc, char *argv[]) {

    int l, m, n, k, a, b, c, d, trials;
    bool skip_vec, skip_simple, skip_gemm, skip_mult;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("l", po::value<int>(&l)->default_value(1048576),
                    "length of vectors in vector addition test")
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
        ("skip-vec", po::value<bool>(&skip_vec)->default_value(false),
            "skip vectors Test")
        ("skip-simple", po::value<bool>(&skip_simple)->default_value(false),
            "skip simple Test")
        ("skip-gemm", po::value<bool>(&skip_gemm)->default_value(false),
            "skip gemm Tests")
        ("skip-mult", po::value<bool>(&skip_mult)->default_value(false),
            "skip mulDense Test")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    runEigenTests(trials, l, m, n, k, a, b, c, d, skip_vec, skip_simple, skip_gemm, skip_mult);

    return 0;
}
