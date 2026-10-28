# RMSE of finite bounded paired regression targets and predictions.
root_mean_squared_error <- function(target, prediction) {
  if (!is.numeric(target) || !is.numeric(prediction) ||
      is.complex(target) || is.complex(prediction) ||
      !is.null(dim(target)) || !is.null(dim(prediction)) ||
      length(target)<1L || length(target)>10000L ||
      length(target)!=length(prediction) ||
      any(!is.finite(target)) || any(!is.finite(prediction)) ||
      any(abs(target)>1000000) || any(abs(prediction)>1000000)) {
    stop("invalid finite bounded paired regression data")
  }
  d<-as.double(prediction)-as.double(target)
  as.double(sqrt(mean(d*d)))
}
