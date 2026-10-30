# Weighted mean absolute regression error for finite bounded vectors.
weighted_mean_absolute_error <- function(targets, predictions, weights) {
  valid <- function(x) is.numeric(x) && !is.complex(x) && all(is.finite(x))
  n <- length(targets)
  if (n==0L || n>4096L || length(predictions)!=n || length(weights)!=n ||
      !valid(targets) || !valid(predictions) || !valid(weights) ||
      any(abs(targets)>1e6) || any(abs(predictions)>1e6) ||
      any(weights<0 | weights>1e6) || sum(weights)<=0) {
    stop("invalid targets, predictions or weights")
  }
  sum(as.double(weights) * abs(as.double(targets)-as.double(predictions))) / sum(weights)
}
