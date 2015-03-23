#include <iostream>
#include <ctime>  
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "boost/program_options.hpp"
#include "boost/random.hpp"

using namespace std;
namespace po = boost::program_options;
using namespace boost::numeric::ublas;

typedef matrix<double, column_major> uBlasMat;
typedef vector<double, column_major> uBlasVec;
typedef boost::minstd_rand base_generator_type;
typedef boost::variate_generator<base_generator_type&, boost::uniform_real<> > rng;


uBlasMat initMatrix(uBlasMat &mat, rng &rnd) {

  for (unsigned i = 0; i < mat.size1(); ++i) {
    for (unsigned j = 0; j < mat.size2(); ++j) {
      mat (i, j) = rnd();
    }
  }

  return mat;
}

uBlasVec initVector(uBlasVec &vec, rng &rnd) {

  for (unsigned i = 0; i < mat.size(); ++i) {
      vec (i) = rnd();
  }

  return vec;
}

// m: numRows, n: numCols
inline double simpleDenseTest_UBlas(int m, int n, int num_trials) {
  base_generator_type generator(42u);
  boost::uniform_real<> uni_dist(0,1);
  rng uni(generator, uni_dist);

  uBlasMat A(m, n); initMatrix(A, uni);
  uBlasMat B(m, n); initMatrix(B, uni);
  uBlasMat C(m, n); initMatrix(C, uni);
  uBlasMat D(m, n); initMatrix(D, uni);
  uBlasMat E(m, n); initMatrix(E, uni);

  clock_t start;
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    uBlasMat res = element_prod((element_div((A + B), C) - D), E);

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  return duration / num_trials;

}

// m: numRows of A, n: numCols of A, and numRows of B, k: numCols of B
inline double gemmSanityTest_UBlas(int m, int n, int k, int num_trials) {

  base_generator_type generator(42u);
  boost::uniform_real<> uni_dist(0,1);
  rng uni(generator, uni_dist);
  uBlasMat A(m, n); initMatrix(A, uni);
  uBlasMat C(n, k); initMatrix(C, uni);
  uBlasMat E(m, k); initMatrix(E, uni);

  clock_t start;
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    E += prod(A, C);

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  return duration / num_trials;
}

// m: numRows of A, n: numCols of A, and numRows of B, k: numCols of B
inline double gemmDenseTest_UBlas(int m, int n, int k, int num_trials) {

  base_generator_type generator(42u);
  boost::uniform_real<> uni_dist(0,1);
  rng uni(generator, uni_dist);
  uBlasMat A(m, n); initMatrix(A, uni);
  uBlasMat B(m, n); initMatrix(B, uni);
  uBlasMat C(n, k); initMatrix(C, uni);
  uBlasMat D(n, k); initMatrix(D, uni);
  uBlasMat E(m, k); initMatrix(E, uni);

  clock_t start;
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    E += prod((A + B), (C - D));

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  return duration / num_trials;
}

inline double mulDenseTest_UBlas(int a, int b, int c, int d, int num_trials) {

  base_generator_type generator(42u);
  boost::uniform_real<> uni_dist(0,1);
  rng uni(generator, uni_dist);
  uBlasMat A(a, a); initMatrix(A, uni);
  uBlasMat B(a, b); initMatrix(B, uni);
  uBlasMat C(b, c); initMatrix(C, uni);
  uBlasMat D(c, d); initMatrix(D, uni);

  clock_t start;
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    uBlasMat res = uBlasMat(prod(uBlasMat(prod(uBlasMat(prod(A, B)), C)), D));

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  return duration / num_trials;
}

// m: numRows of A, n: numCols of A, and numRows of B, k: numCols of B
inline double denseVectorTest_UBlas(int l, int num_trials) {

  base_generator_type generator(42u);
  boost::uniform_real<> uni_dist(0,1);
  rng uni(generator, uni_dist);
  uBlasVec a(l); initVector(a, uni);
  uBlasVec b(l); initVector(b, uni);
  uBlasVec c(l); initVector(c, uni);
  uBlasVec d(l); initVector(d, uni);
  uBlasVec res(l);

  clock_t start;
  start = clock();
  for (unsigned i = 0; i < num_trials; i++) {

    res = a + b + c + d;

  }
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

  return duration / num_trials;
}

void runUBlasTests(int num_trials, int l, int m, int n, int k, int a, int b, int c, int d) {

    cout << "UBlas Vectors Test:\t" << denseVectorTest_UBlas(l, num_trials) << endl;
    cout << "UBlas Simple Test:\t" << simpleDenseTest_UBlas(m, n, num_trials) << endl;
    cout << "UBlas gemmSanity Test:\t" << gemmDenseTest_UBlas(m, n, k, num_trials) << endl;
    cout << "UBlas gemm Test:\t" << gemmDenseTest_UBlas(m, n, k, num_trials) << endl;
    cout << "UBlas mulDense Test:\t" << mulDenseTest_UBlas(a, b, c, d, num_trials) << endl;

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
    int l;

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
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    runUBlasTests(trials, l, m, n, k, a, b, c, d);

    return 0;
}
