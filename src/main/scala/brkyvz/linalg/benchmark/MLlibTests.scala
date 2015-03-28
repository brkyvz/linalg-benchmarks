package brkyvz.linalg.benchmark

import java.util.Random

import brkyvz.linalg.mllib.{Vectors, DenseMatrix}
import brkyvz.linalg.Params

object MLlibTests {

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
