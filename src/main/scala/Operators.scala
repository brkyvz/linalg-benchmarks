
package brkyvz.linalg

object VectorOperators {

  def add(x: VectorLike, y: VectorLike): LazyVector = new LazyDenseVVOp(x, y, _ + _, None)
  def sub(x: VectorLike, y: VectorLike): LazyVector = new LazyDenseVVOp(x, y, _ - _, None)
  def mul(x: VectorLike, y: VectorLike): LazyVector = new LazyDenseVVOp(x, y, _ * _, None)
  def div(x: VectorLike, y: VectorLike): LazyVector = new LazyDenseVVOp(x, y, _ / _, None)
  def addInto(x: VectorLike, y: VectorLike, into: DenseVector): LazyVector =
    new LazyDenseVVOp(x, y, _ + _, Option(into))
  def subInto(x: VectorLike, y: VectorLike, into: DenseVector): LazyVector =
    new LazyDenseVVOp(x, y, _ - _, Option(into))
  def mulInto(x: VectorLike, y: VectorLike, into: DenseVector): LazyVector =
    new LazyDenseVVOp(x, y, _ * _, Option(into))
  def divInto(x: VectorLike, y: VectorLike, into: DenseVector): LazyVector =
    new LazyDenseVVOp(x, y, _ / _, Option(into))
  
  def log(x: VectorLike): LazyVector = new LazyDenseVOp(x, math.log, None)
  
  implicit class VectorLikeDouble(x: Double) extends VectorLike {
    def apply(i: Int): Double = x
    def size = 1
  }

  implicit class VectorLikeInt(x: Int) extends VectorLike {
    def apply(i: Int): Double = x * 1.0
    def size = 1
  }
}

object MatrixOperators {



  implicit class MatrixLikeDouble(x: Double) {
    
  }

  implicit class MatrixLikeInt(x: Int) {
  }
}
