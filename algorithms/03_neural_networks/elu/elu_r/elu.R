# Exponential linear unit with alpha one.
elu <- function(values) {
  if (!is.numeric(values) || is.complex(values) || !is.null(dim(values)) ||
      length(values)<1L || length(values)>10000L ||
      any(!is.finite(values)) || any(abs(values)>1000000)) {
    stop("finite bounded numeric vector required")
  }
  x <- as.double(values)
  result <- x
  negative <- x<0
  result[negative] <- exp(x[negative])-1
  as.double(result)
}
