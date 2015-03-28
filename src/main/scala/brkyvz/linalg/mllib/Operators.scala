
package brkyvz.linalg.mllib

object VectorOperators {

  def add(x: VectorLike, y: VectorLike): LazyVector = new LazyDenseVVOp(x, y, _ + _, None)
  def sub(x: VectorLike, y: VectorLike): LazyVector = new LazyDenseVVOp(x, y, _ - _, None)
  def mul(x: VectorLike, y: VectorLike): LazyVector = new LazyDenseVVOp(x, y, _ * _, None)
  def div(x: VectorLike, y: VectorLike): LazyVector = new LazyDenseVVOp(x, y, _ / _, None)
  
  def log(x: VectorLike): LazyVector = new LazyDenseVOp(x, math.log, None)
  
  implicit class VectorLikeDouble(x: Double) extends VectorLike {
    def apply(i: Int): Double = x
    def size = 1
  }

  implicit class VectorLikeInt(x: Int) extends VectorLikeDouble(x * 1.0)
}

object MatrixOperators {

  def add(x: MatrixLike, y: MatrixLike): LazyMatrix = new LazyImDenseMMOp(x, y, _ + _)
  def sub(x: MatrixLike, y: MatrixLike): LazyMatrix = new LazyImDenseMMOp(x, y, _ - _)
  def mul(x: MatrixLike, y: MatrixLike): LazyMatrix = new LazyImDenseMMOp(x, y, _ * _)
  def mul(x: MatrixLikeDouble, y: Matrix): LazyMatrix = new LazyImDenseScaleOp(x, y)
  def mul(x: Matrix, y: MatrixLikeDouble): LazyMatrix = new LazyImDenseScaleOp(y, x)
  def div(x: MatrixLike, y: MatrixLike): LazyMatrix = new LazyImDenseMMOp(x, y, _ / _)
  def log(x: Matrix): LazyMatrix = new LazyImDenseMOp(x, math.log)

  implicit class MatrixLikeDouble(val value: Double) extends MatrixLike {
    def apply(i: Int): Double = value
    override def numRows = 1
    override def numCols = 1
    override def size = 1
    
    def *(x: Matrix): LazyMatrix = new LazyImDenseScaleOp(value, x)
    def :*(x: Matrix): LazyMatrix = new LazyImDenseScaleOp(value, x)
  }

  implicit class MatrixLikeInt(x: Int) extends MatrixLikeDouble(x * 1.0)
}
