# Cosine similarity between two finite, nonzero real vectors.
cosine_similarity <- function(left, right) {
  if (!is.numeric(left) || !is.numeric(right) ||
      is.complex(left) || is.complex(right)) {
    stop("vectors must contain real numbers")
  }
  if (length(left) != length(right) || length(left) == 0L) {
    stop("vectors must be nonempty and have equal lengths")
  }
  if (any(!is.finite(left)) || any(!is.finite(right))) {
    stop("components must be finite")
  }
  a_max <- max(abs(left))
  b_max <- max(abs(right))
  if (a_max == 0 || b_max == 0) {
    stop("cosine similarity is undefined for zero vectors")
  }
  x <- as.double(left) / a_max
  y <- as.double(right) / b_max
  score <- sum(x * y) / (sqrt(sum(x * x)) * sqrt(sum(y * y)))
  if (!is.finite(score)) stop("similarity is not finite")
  max(-1.0, min(1.0, as.double(score)))
}
