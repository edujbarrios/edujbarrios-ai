# Mean squared log1p differences for nonnegative regression data.
mean_squared_log_error <- function(target,prediction) {
  if (!is.numeric(target) || !is.numeric(prediction) ||
      is.complex(target) || is.complex(prediction) ||
      !is.null(dim(target)) || !is.null(dim(prediction)) ||
      length(target)<1L || length(target)>10000L ||
      length(target)!=length(prediction) ||
      any(!is.finite(target)) || any(!is.finite(prediction)) ||
      any(target<0) || any(prediction<0) ||
      any(target>1000000) || any(prediction>1000000)) {
    stop("paired finite nonnegative regression data required")
  }
  a<-log1p(as.double(target))
  b<-log1p(as.double(prediction))
  as.double(mean((b-a)^2))
}
