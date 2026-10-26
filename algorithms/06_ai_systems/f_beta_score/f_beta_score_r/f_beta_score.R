# Stable positive-class F-beta score for binary confusion counts.
f_beta_score <- function(tp, fp, fn, beta = 1.0) {
  counts <- c(tp, fp, fn)
  if (!is.numeric(tp) || !is.numeric(fp) || !is.numeric(fn) ||
      length(tp) != 1L || length(fp) != 1L || length(fn) != 1L ||
      length(counts) != 3L || any(!is.finite(counts)) ||
      any(counts < 0 | counts > 2147483647) ||
      any(counts != trunc(counts)) ||
      !is.numeric(beta) || length(beta) != 1L ||
      !is.finite(beta) || beta <= 0) {
    stop("nonnegative signed 32-bit counts and positive finite beta required")
  }
  if (tp == 0) return(0.0)
  precision <- tp / (tp + fp)
  recall <- tp / (tp + fn)
  if (beta <= 1) {
    b2 <- beta * beta
    weight <- b2 / (1 + b2)
  } else {
    inverse <- 1 / beta
    weight <- 1 / (1 + inverse * inverse)
  }
  precision * recall / (weight * precision + (1 - weight) * recall)
}
