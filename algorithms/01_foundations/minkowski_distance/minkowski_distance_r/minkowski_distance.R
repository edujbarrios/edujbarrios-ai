# Minkowski L-p distance with scale-normalized residuals.
minkowski_distance <- function(first, second, p = 2) {
  check <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        length(x) == 0L || any(!is.finite(x))) {
      stop("nonempty finite real vector required")
    }
    as.double(x)
  }
  if (!is.numeric(p) || is.complex(p) || length(p) != 1L ||
      !is.finite(p) || p < 1) stop("finite p >= 1 required")
  a <- check(first)
  b <- check(second)
  if (length(a) != length(b)) stop("mismatched lengths")
  d <- abs(a - b)
  if (any(!is.finite(d))) stop("unrepresentable difference")
  scale <- max(d)
  if (scale == 0) return(0)
  answer <- scale * sum((d / scale)^p)^(1 / p)
  if (!is.finite(answer)) stop("unrepresentable distance")
  answer
}
