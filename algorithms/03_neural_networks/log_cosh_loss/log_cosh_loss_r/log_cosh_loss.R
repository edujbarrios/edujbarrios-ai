# Mean regression log-cosh loss evaluated without overflowing cosh.
log_cosh_loss <- function(target, prediction) {
  if (!is.numeric(target) || !is.numeric(prediction) ||
      is.complex(target) || is.complex(prediction) ||
      !is.null(dim(target)) || !is.null(dim(prediction)) ||
      length(target)<1L || length(target)>10000L ||
      length(target)!=length(prediction) ||
      any(!is.finite(target)) || any(!is.finite(prediction)) ||
      any(abs(target)>1000000) || any(abs(prediction)>1000000)) {
    stop("invalid paired regression inputs")
  }
  magnitude <- abs(as.double(prediction)-as.double(target))
  term <- pmax(0, magnitude + log1p(exp(-2*magnitude)) - log(2))
  as.double(mean(term))
}
