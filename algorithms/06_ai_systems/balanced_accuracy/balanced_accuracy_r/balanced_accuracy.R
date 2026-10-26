# Balanced accuracy of aligned binary labels, both truth classes required.
balanced_accuracy <- function(truth, predictions) {
  valid <- function(x) is.numeric(x) && !is.complex(x) &&
    all(is.finite(x)) && all(x %in% c(0, 1))
  if (!valid(truth) || !valid(predictions) ||
      length(truth) == 0L || length(truth) != length(predictions)) {
    stop("nonempty aligned binary labels required")
  }
  positives <- sum(truth == 1)
  negatives <- sum(truth == 0)
  if (positives == 0L || negatives == 0L) stop("both truth classes required")
  tp <- sum(truth == 1 & predictions == 1)
  tn <- sum(truth == 0 & predictions == 0)
  as.double(0.5 * (tp / positives + tn / negatives))
}
