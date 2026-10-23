# Overflow-safe logistic sigmoid, elementwise.
sigmoid <- function(values) {
  if (!is.numeric(values) || is.complex(values) ||
      any(!is.finite(values))) {
    stop("components must be finite real numbers")
  }
  output <- numeric(length(values))
  for (i in seq_along(values)) {
    x <- as.double(values[[i]])
    if (x >= 0.0) {
      output[[i]] <- 1.0 / (1.0 + exp(-x))
    } else {
      ex <- exp(x)
      output[[i]] <- ex / (1.0 + ex)
    }
  }
  output
}
