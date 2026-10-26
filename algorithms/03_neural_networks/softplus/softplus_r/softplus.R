# Numerically stable Softplus and its sigmoid derivative.
softplus <- function(inputs) {
  if (!is.numeric(inputs) || is.complex(inputs) ||
      length(inputs) == 0L || any(!is.finite(inputs))) {
    stop("nonempty finite real vector required")
  }
  values <- pmax(inputs, 0) + log1p(exp(-abs(inputs)))
  z <- exp(-abs(inputs))
  derivatives <- ifelse(inputs >= 0, 1 / (1 + z), z / (1 + z))
  list(values = as.numeric(values), derivatives = as.numeric(derivatives))
}
