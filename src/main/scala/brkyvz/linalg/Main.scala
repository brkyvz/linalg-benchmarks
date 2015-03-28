
package brkyvz.linalg

import brkyvz.linalg.benchmark._
import scopt.OptionParser

object Main {

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
      MLlibTests.run(params)
    } getOrElse {
      System.exit(1)
    }
  }

}

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
