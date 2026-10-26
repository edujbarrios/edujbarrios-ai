# Scaled Bray-Curtis dissimilarity for nonnegative feature vectors.
bray_curtis_dissimilarity <- function(first, second) {
  checked <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        length(x) == 0L || any(!is.finite(x)) || any(x < 0)) {
      stop("nonempty finite nonnegative feature vector required")
    }
    as.double(x)
  }
  a <- checked(first)
  b <- checked(second)
  if (length(a) != length(b)) stop("feature lengths differ")
  scale <- max(c(a, b))
  if (scale == 0) return(0.0)
  x <- a / scale
  y <- b / scale
  answer <- sum(abs(x - y)) / sum(x + y)
  if (!is.finite(answer)) stop("nonfinite result")
  min(1.0, max(0.0, answer))
}
