# Weighted binary Brier score for real numeric target/probability/weight vectors.
weighted_brier_score <- function(targets, probabilities, weights) {
  valid <- function(x) is.numeric(x) && !is.complex(x) && all(is.finite(x))
  n <- length(targets)
  if (n < 1L || n > 4096L ||
      length(probabilities) != n || length(weights) != n ||
      !valid(targets) || !valid(probabilities) || !valid(weights) ||
      any(targets != 0 & targets != 1) ||
      any(probabilities < 0 | probabilities > 1) ||
      any(weights < 0 | weights > 1e6) || sum(weights) <= 0) {
    stop("invalid binary targets, probabilities or weights")
  }
  sum(as.double(weights) * (as.double(probabilities) - as.double(targets))^2) / sum(weights)
}
