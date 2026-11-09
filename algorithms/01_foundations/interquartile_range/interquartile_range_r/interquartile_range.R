# Interquartile range with R-default (type 7) interpolated quartiles.
interquartile_range <- function(values) {
  if (!is.numeric(values) || is.logical(values) || is.complex(values) ||
      !is.null(dim(values)) || length(values)<1L || length(values)>512L ||
      any(!is.finite(values))) {
    stop("expected 1..512 finite real observations")
  }
  x <- sort(as.double(values))
  n <- length(x)
  quantile7 <- function(p) {
    h <- (n-1)*p
    i <- as.integer(floor(h))+1L
    f <- h-floor(h)
    if (f==0) return(x[[i]])
    (1-f)*x[[i]]+f*x[[i+1L]]
  }
  result <- quantile7(0.75)-quantile7(0.25)
  if (!is.finite(result)) stop("nonfinite interquartile range")
  as.double(max(0,result))
}
