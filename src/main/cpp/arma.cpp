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
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    mat res = ((A + B) / C - D) % E;

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  return duration / num_trials;

}

// m: numRows of A, n: numCols of A, and numRows of B, k: numCols of B
inline double gemmSanityTest_Arma(int m, int n, int k, int num_trials) {

  mat A = randu<mat>(m, n);
  mat C = randu<mat>(n, k);
  mat E = randu<mat>(m, k);

  clock_t start;
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    E += A * C;

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

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
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    E += (A + B) * (C - D);

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  
  return duration / num_trials;

}

inline double mulDenseTest_Arma(int a, int b, int c, int d, int num_trials) {

  mat A = randu<mat>(a, a);
  mat B = randu<mat>(a, b);
  mat C = randu<mat>(b, c);
  mat D = randu<mat>(c, d);

  clock_t start;
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    mat res = A * B * C * D;

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  return duration / num_trials;
}

// m: numRows of A, n: numCols of A, and numRows of B, k: numCols of B
inline double denseVectorTest_Arma(int l, int num_trials) {

  vec a = randu<vec>(l);
  vec b = randu<vec>(l);
  vec c = randu<vec>(l);
  vec d = randu<vec>(l);
  vec e = randu<vec>(l);

  clock_t start;
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {
    e = a + b + c + d;
  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

  return duration / num_trials;
}

void runArmaTests(int num_trials, int l, int m, int n, int k, int a, int b, int c, int d, 
    bool skip_vec, bool skip_simple, bool skip_gemm, bool skip_mult) {

  if (!skip_vec)
    cout << "Armadillo Vectors Test:\t" << denseVectorTest_Arma(l, num_trials) << endl;
  if (!skip_simple)
    cout << "Armadillo Simple Test:\t" << simpleDenseTest_Arma(m, n, num_trials) << endl;
  if (!skip_gemm) {
    cout << "Armadillo gemmSanity Test:\t" << gemmDenseTest_Arma(m, n, k, num_trials) << endl;
    cout << "Armadillo gemm Test:\t" << gemmDenseTest_Arma(m, n, k, num_trials) << endl;
  }
  if (!skip_mult)
    cout << "Armadillo mulDense Test:\t" << mulDenseTest_Arma(a, b, c, d, num_trials) << endl;

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

    runArmaTests(trials, l, m, n, k, a, b, c, d, skip_vec, skip_simple, skip_gemm, skip_mult);

    return 0;
}
