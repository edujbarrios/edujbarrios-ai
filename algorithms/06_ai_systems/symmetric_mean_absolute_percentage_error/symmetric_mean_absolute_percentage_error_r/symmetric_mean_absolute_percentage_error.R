# Symmetric MAPE returns an unweighted ratio in 0..2, not percent.
symmetric_mean_absolute_percentage_error <- function(target,prediction) {
  if (!is.numeric(target) || !is.numeric(prediction) ||
      is.complex(target) || is.complex(prediction) ||
      !is.null(dim(target)) || !is.null(dim(prediction)) ||
      length(target)<1L || length(target)>10000L ||
      length(target)!=length(prediction) ||
      any(!is.finite(target)) || any(!is.finite(prediction)) ||
      any(abs(target)>1000000) || any(abs(prediction)>1000000)) {
    stop("invalid paired target or prediction values")
  }
  t <- as.double(target)
  p <- as.double(prediction)
  denom <- abs(t)+abs(p)
  terms <- numeric(length(t))
  nonzero <- denom>0
  terms[nonzero] <- 2*abs(p[nonzero]-t[nonzero])/denom[nonzero]
  as.double(max(0,min(2,mean(terms))))
}
