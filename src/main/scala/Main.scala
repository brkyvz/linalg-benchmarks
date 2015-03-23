
package brkyvz.linalg

import scopt.OptionParser
import brkyvz.linalg._
import java.util.Random

object Main {

  case class Params(
    m: Int = 1024,
    n: Int = 1024,
    trials: Int = 10,
    warmup: Int = 3,
    k: Int = 1024,
    a: Int = 1024,
    b: Int = 512,
    c: Int = 256,
    d: Int = 128)

  def main(args: Array[String]) {
    val defaultParams = Params()

    val parser = new OptionParser[Params]("MovieLensALS") {
      head("MovieLensALS: an example app for ALS on MovieLens data.")
      opt[Int]("m")
        .text(s"m, default: ${defaultParams.m}}")
        .action((x, c) => c.copy(m = x))
      opt[Int]("n")
        .text(s"number of iterations, default: ${defaultParams.n}")
        .action((x, c) => c.copy(n = x))
      opt[Int]("trials")
        .text(s"lambda (smoothing constant), default: ${defaultParams.trials}")
        .action((x, c) => c.copy(trials = x))
      opt[Int]("warmup")
        .text("use Kryo serialization")
        .action((x, c) => c.copy(warmup = x))
      opt[Int]("k")
        .text(s"number of user blocks, default: ${defaultParams.k} (auto)")
        .action((x, c) => c.copy(k = x))
      opt[Int]("a")
        .text(s"number of user blocks, default: ${defaultParams.a} (auto)")
        .action((x, c) => c.copy(a = x))
      opt[Int]("b")
        .text(s"number of user blocks, default: ${defaultParams.b} (auto)")
        .action((x, c) => c.copy(b = x))
      opt[Int]("c")
        .text(s"number of user blocks, default: ${defaultParams.c} (auto)")
        .action((x, f) => f.copy(c = x))
      opt[Int]("d")
        .text(s"number of user blocks, default: ${defaultParams.d} (auto)")
        .action((x, c) => c.copy(d = x))
    }

    parser.parse(args, defaultParams).map { params =>
      run(params)
    } getOrElse {
      System.exit(1)
    }
  }
  
  def run(params: Params): Unit = {
    println(s"MLlib Simple Test:\t${simpleDenseTest_MLlib(params.m, params.n, params.trials, params.warmup)}")
  }
  
  def simpleDenseTest_MLlib(m: Int, n: Int, trials: Int, warmup: Int): Double = {

    val rand = new Random()
    
    val A = DenseMatrix.rand(m, n, rand)
    val B = DenseMatrix.rand(m, n, rand)
    val C = DenseMatrix.rand(m, n, rand)
    val D = DenseMatrix.rand(m, n, rand)
    val E = DenseMatrix.rand(m, n, rand)

    var duration = 0.0

    var i = 0
    while (i < warmup) {
      val res = ((A + B) / C - D) :* E
      res.compute()
      i += 1
    }
    System.gc()
    
    i = 0
    while (i < trials) {
      val start = System.nanoTime()

      val res = ((A + B) / C - D) :* E
      res.compute()
      duration += ( System.nanoTime() - start )

      i += 1
    }
    
    duration / trials / 1e9
  }
}
