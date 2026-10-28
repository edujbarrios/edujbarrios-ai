# Forward softsign activation, x/(1+abs(x)).
softsign <- function(values) {
  if (!is.numeric(values) || is.complex(values) || !is.null(dim(values)) ||
      length(values)<1L || length(values)>10000L ||
      any(!is.finite(values)) || any(abs(values)>1000000)) {
    stop("bounded finite numeric vector required")
  }
  x <- as.double(values)
  as.double(x/(1+abs(x)))
}
