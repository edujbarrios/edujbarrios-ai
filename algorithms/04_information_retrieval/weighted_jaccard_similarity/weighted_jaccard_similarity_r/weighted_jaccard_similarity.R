# Scaled min/max weighted Jaccard similarity for aligned real features.
weighted_jaccard_similarity <- function(first, second) {
  check <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        length(x) == 0L || any(!is.finite(x)) || any(x < 0)) {
      stop("nonempty finite nonnegative vector required")
    }
    as.double(x)
  }
  a <- check(first)
  b <- check(second)
  if (length(a) != length(b)) stop("dimensions differ")
  scale <- max(c(a, b))
  if (scale == 0) return(1.0)
  numerator <- sum(pmin(a, b) / scale)
  denominator <- sum(pmax(a, b) / scale)
  score <- numerator / denominator
  if (!is.finite(score)) stop("nonfinite similarity")
  min(1.0, max(0.0, score))
}
