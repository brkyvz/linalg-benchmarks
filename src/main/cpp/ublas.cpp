#include <iostream>
#include <ctime>  
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include "boost/program_options.hpp"
#include "boost/random.hpp"

using namespace std;
namespace po = boost::program_options;
using namespace boost::numeric::ublas;

typedef matrix<double, column_major> uBlasMat;
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
  double duration = 0.0;
  
  for (unsigned i = 0; i < num_trials; i++) {

    start = clock();

    uBlasMat res = element_prod((element_div((A + B), C) - D), E);

    duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  }
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
  double duration = 0.0;
  
  for (unsigned i = 0; i < num_trials; i++) {
    start = clock();

    E += prod((A + B), (C - D));

    duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  }

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
  double duration = 0.0;
  
  for (unsigned i = 0; i < num_trials; i++) {
    start = clock();

    uBlasMat res = uBlasMat(prod(uBlasMat(prod(uBlasMat(prod(A, B)), C)), D));

    duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  }

  return duration / num_trials;
}

void runUBlasTests(int num_trials, int m, int n, int k, int a, int b, int c, int d) {

   cout << "UBlas Simple Test:\t" << simpleDenseTest_UBlas(m, n, num_trials) << endl;
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

    runUBlasTests(trials, m, n, k, a, b, c, d);

    return 0;
}
