# Dense matrix multiplication without calling %*% or third-party packages.
matrix_multiplication <- function(left, right) {
  if (!is.matrix(left) || !is.matrix(right) ||
      !is.numeric(left) || !is.numeric(right) ||
      is.complex(left) || is.complex(right)) {
    stop("inputs must be real numeric matrices")
  }
  m <- nrow(left)
  k <- ncol(left)
  n <- ncol(right)
  if (m == 0L || k == 0L || n == 0L || nrow(right) != k) {
    stop("matrix dimensions must be positive and compatible")
  }
  if (any(!is.finite(left)) || any(!is.finite(right))) {
    stop("matrix entries must be finite")
  }
  output <- matrix(0.0, nrow=m, ncol=n)
  for (i in seq_len(m)) {
    for (j in seq_len(n)) {
      total <- 0.0
      for (p in seq_len(k)) {
        product <- as.double(left[i, p]) * as.double(right[p, j])
        total <- total + product
        if (!is.finite(product) || !is.finite(total)) {
          stop("matrix multiplication overflow")
        }
      }
      output[i, j] <- total
    }
  }
  output
}
