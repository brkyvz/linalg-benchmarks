
package brkyvz.linalg

import scopt.OptionParser
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
    d: Int = 128,
    l: Int = 1048576,
    skipVector: Boolean = false,
    skipSimple: Boolean = false,
    skipGemm: Boolean = false,
    skipMult: Boolean = false)

  def main(args: Array[String]) {
    val defaultParams = Params()

    val parser = new OptionParser[Params]("MovieLensALS") {
      head("MovieLensALS: an example app for ALS on MovieLens data.")
      opt[Int]("m")
        .text(s"m, default: ${defaultParams.m}")
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
      opt[Int]("l")
        .text(s"length of vectors, default: ${defaultParams.l} (auto)")
        .action((x, c) => c.copy(l = x))
      opt[Unit]("skip-vec")
        .text("use Kryo serialization")
        .action((_, c) => c.copy(skipVector = true))
      opt[Unit]("skip-simple")
        .text("use Kryo serialization")
        .action((_, c) => c.copy(skipSimple = true))
      opt[Unit]("skip-gemm")
        .text("use Kryo serialization")
        .action((_, c) => c.copy(skipGemm = true))
      opt[Unit]("skip-mult")
        .text("use Kryo serialization")
        .action((_, c) => c.copy(skipMult = true))
    }

    parser.parse(args, defaultParams).map { params =>
      run(params)
    } getOrElse {
      System.exit(1)
    }
  }
  
  def run(params: Params): Unit = {
    if (!params.skipVector)
      println(s"MLlib Vectors Test:\t${denseVectorTest_MLlib(params.l, params.trials, params.warmup)}")
    if (!params.skipSimple)
      println(s"MLlib Simple Test:\t${simpleDenseTest_MLlib(params.m, params.n, params.trials, params.warmup)}")
    if (!params.skipGemm) {
      println(s"MLlib gemm Sanity Test:\t${gemmDenseTest_MLlib(params.m, params.n, params.k, params.trials, params.warmup)}")
      println(s"MLlib gemm Test:\t${gemmDenseTest_MLlib(params.m, params.n, params.k, params.trials, params.warmup)}")
    }
    if (!params.skipMult)
      println(s"MLlib mulDense Test:\t${mulDenseTest_MLlib(params.a, params.b, params.c, params.d, params.trials, params.warmup)}")
  }
  
  def simpleDenseTest_MLlib(m: Int, n: Int, trials: Int, warmup: Int): Double = {

    val rand = new Random()
    
    val A = DenseMatrix.rand(m, n, rand)
    val B = DenseMatrix.rand(m, n, rand)
    val C = DenseMatrix.rand(m, n, rand)
    val D = DenseMatrix.rand(m, n, rand)
    val E = DenseMatrix.rand(m, n, rand)

    var i = 0
    while (i < warmup) {
      val res = ((A + B) / C - D) :* E
      res.compute()
      i += 1
    }
    System.gc()
    
    i = 0
    val start = System.nanoTime()
    while (i < trials) {
      val res = ((A + B) / C - D) :* E
      res.compute()
      i += 1
    }
    val duration = System.nanoTime() - start
    
    duration / trials / 1e9
  }

  def gemmSanityTest_MLlib(m: Int, n: Int, k: Int, trials: Int, warmup: Int): Double = {

    val rand = new Random()

    val A = DenseMatrix.rand(m, n, rand)
    val C = DenseMatrix.rand(n, k, rand)
    val E = DenseMatrix.rand(m, k, rand)

    var i = 0
    while (i < warmup) {
      E += A * C
      i += 1
    }
    System.gc()

    i = 0
    val start = System.nanoTime()
    while (i < trials) {
      E += A * C
      i += 1
    }
    val duration = System.nanoTime() - start

    duration / trials / 1e9
  }

  def gemmDenseTest_MLlib(m: Int, n: Int, k: Int, trials: Int, warmup: Int): Double = {

    val rand = new Random()

    val A = DenseMatrix.rand(m, n, rand)
    val B = DenseMatrix.rand(m, n, rand)
    val C = DenseMatrix.rand(n, k, rand)
    val D = DenseMatrix.rand(n, k, rand)
    val E = DenseMatrix.rand(m, k, rand)

    var i = 0
    while (i < warmup) {
      E += (A + B) * (C - D)
      i += 1
    }
    System.gc()

    i = 0
    val start = System.nanoTime()
    while (i < trials) {
      E += (A + B) * (C - D)
      
      i += 1
    }
    val duration = System.nanoTime() - start

    duration / trials / 1e9
  }

  def mulDenseTest_MLlib(a: Int, b: Int, c: Int, d: Int, trials: Int, warmup: Int): Double = {

    val rand = new Random()

    val A = DenseMatrix.rand(a, a, rand)
    val B = DenseMatrix.rand(a, b, rand)
    val C = DenseMatrix.rand(b, c, rand)
    val D = DenseMatrix.rand(c, d, rand)

    var i = 0
    while (i < warmup) {
      val res = A * B * C * D
      res.compute()
      i += 1
    }
    System.gc()

    i = 0
    val start = System.nanoTime()
    while (i < trials) {
      val res = A * B * C * D
      res.compute()
      
      i += 1
    }
    val duration = System.nanoTime() - start

    duration / trials / 1e9
  }

  def denseVectorTest_MLlib(l: Int, trials: Int, warmup: Int): Double = {

    val rand = new Random()

    val a = Vectors.dense(Array.fill(l)(rand.nextDouble()))
    val b = Vectors.dense(Array.fill(l)(rand.nextDouble()))
    val c = Vectors.dense(Array.fill(l)(rand.nextDouble()))
    val d = Vectors.dense(Array.fill(l)(rand.nextDouble()))

    var i = 0
    while (i < warmup) {
      val res = a + b + c + d
      res.compute()
      i += 1
    }
    System.gc()

    i = 0
    val start = System.nanoTime()
    while (i < trials) {
      val res = a + b + c + d
      res.compute()
      
      i += 1
    }
    val duration = System.nanoTime() - start

    duration / trials / 1e9
  }
}
