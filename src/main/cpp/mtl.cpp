#include <iostream>
#include <cstdio>
#include <ctime>
#include <boost/numeric/mtl/mtl.hpp>
#include "boost/program_options.hpp"


using namespace std;
using namespace mtl;
using namespace mtl::mat;
namespace po = boost::program_options;

// m: numRows, n: numCols
inline double simpleDenseTest_MTL(int m, int n, int num_trials) {

  dense2D<double, parameters<tag::col_major> > A(m, n); random(A);
  dense2D<double, parameters<tag::col_major> > B(m, n); random(B);
  dense2D<double, parameters<tag::col_major> > C(m, n); random(C);
  dense2D<double, parameters<tag::col_major> > D(m, n); random(D);
  dense2D<double, parameters<tag::col_major> > E(m, n); random(E);

  clock_t start;
  double duration = 0.0;
      
  for (unsigned i = 0; i < num_trials; i++) {
    start = clock();

    (ele_div((A + B), C) - D) * E;

    duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  }
  return duration / num_trials;

}

// m: numRows of A, n: numCols of A, and numRows of B, k: numCols of B
inline double gemmDenseTest_MTL(int m, int n, int k, int num_trials) {

  dense2D<double, parameters<tag::col_major> > A(m, n); random(A);
  dense2D<double, parameters<tag::col_major> > B(m, n); random(B);
  dense2D<double, parameters<tag::col_major> > C(n, k); random(C);
  dense2D<double, parameters<tag::col_major> > D(n, k); random(D);
  dense2D<double, parameters<tag::col_major> > E(m, k); random(E);

  clock_t start;
  double duration = 0.0;
      
  for (unsigned i = 0; i < num_trials; i++) {
    start = clock();

    E += (A + B) * (C - D);

    duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  }
  return duration / num_trials;

}

inline double mulDenseTest_MTL(int a, int b, int c, int d, int num_trials) {

  dense2D<double, parameters<tag::col_major> > A(a, a); random(A);
  dense2D<double, parameters<tag::col_major> > B(a, b); random(B);
  dense2D<double, parameters<tag::col_major> > C(b, c); random(C);
  dense2D<double, parameters<tag::col_major> > D(c, d); random(D);

  clock_t start;
  double duration = 0.0;
      
  for (unsigned i = 0; i < num_trials; i++) {
    start = clock();

    A * B * C * D;

    duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  }

  return duration / num_trials;

}

void runMTLTests(int num_trials, int m, int n, int k, int a, int b, int c, int d) {

   cout << "MTL Simple Test:\t" << simpleDenseTest_MTL(m, n, num_trials) << endl;
   cout << "MTL gemm Test:\t" << gemmDenseTest_MTL(m, n, k, num_trials) << endl;
   cout << "MTL mulDense Test:\t" << mulDenseTest_MTL(a, b, c, d, num_trials) << endl;

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

    runMTLTests(trials, m, n, k, a, b, c, d);

    return 0;
}
