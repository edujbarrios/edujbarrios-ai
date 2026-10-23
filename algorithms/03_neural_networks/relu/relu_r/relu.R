# Elementwise ReLU for finite real numeric vectors.
relu <- function(values) {
  if (!is.numeric(values) || is.complex(values) ||
      any(!is.finite(values))) {
    stop("components must be finite real numbers")
  }
  pmax(as.double(values), 0.0)
}
