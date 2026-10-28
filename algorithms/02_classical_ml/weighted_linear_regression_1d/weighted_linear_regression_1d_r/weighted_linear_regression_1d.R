# One-feature weighted least squares with a free intercept.
weighted_linear_regression_1d <- function(x,y,weights) {
  if (!is.numeric(x) || !is.numeric(y) || !is.numeric(weights) ||
      is.complex(x) || is.complex(y) || is.complex(weights) ||
      !is.null(dim(x)) || !is.null(dim(y)) || !is.null(dim(weights)) ||
      length(x)<2L || length(x)>128L || length(x)!=length(y) ||
      length(x)!=length(weights) ||
      any(!is.finite(x)) || any(!is.finite(y)) || any(!is.finite(weights)) ||
      any(abs(x)>1000000) || any(abs(y)>1000000) ||
      any(weights<0) || any(weights>1000000)) {
    stop("invalid aligned bounded values or weights")
  }
  total<-sum(weights)
  if (total<=0) stop("positive weight sum required")
  mx<-sum(weights*x)/total
  my<-sum(weights*y)/total
  dx<-x-mx
  denom<-sum(weights*dx*dx)
  if (!is.finite(denom) || denom<=0) stop("positive weighted feature variance required")
  slope<-sum(weights*dx*(y-my))/denom
  intercept<-my-slope*mx
  if (!is.finite(slope) || !is.finite(intercept)) stop("nonfinite regression")
  c(slope=as.double(slope),intercept=as.double(intercept))
}
