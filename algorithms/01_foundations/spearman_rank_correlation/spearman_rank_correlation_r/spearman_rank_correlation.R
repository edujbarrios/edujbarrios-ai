# Spearman correlation with explicitly computed average ranks for ties.
spearman_rank_correlation <- function(x, y) {
  if (!is.numeric(x) || !is.numeric(y) || is.complex(x) || is.complex(y) ||
      !is.null(dim(x)) || !is.null(dim(y)) ||
      length(x)<2L || length(x)>128L || length(y)!=length(x) ||
      any(!is.finite(x)) || any(!is.finite(y)) ||
      any(abs(x)>1000000) || any(abs(y)>1000000)) {
    stop("invalid paired numerical observations")
  }
  midranks <- function(values) {
    vapply(values, function(current) {
      as.double(sum(values < current) + (sum(values == current) + 1) / 2)
    }, numeric(1))
  }
  n <- length(x)
  dx <- midranks(x) - (n+1)/2
  dy <- midranks(y) - (n+1)/2
  sxx <- sum(dx*dx)
  syy <- sum(dy*dy)
  if (sxx==0 || syy==0) stop("correlation undefined for constant ranks")
  rho <- (sum(dx*dy)/sqrt(sxx))/sqrt(syy)
  if (!is.finite(rho)) stop("nonfinite correlation")
  max(-1.0,min(1.0,rho))
}
