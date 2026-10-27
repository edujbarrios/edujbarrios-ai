# Numerically stable softmax of logits scaled by positive temperature.
temperature_softmax <- function(logits, temperature) {
  if (!is.numeric(logits) || is.complex(logits) ||
      !is.null(dim(logits)) || length(logits)<1L ||
      length(logits)>4096L || any(!is.finite(logits)) ||
      any(abs(logits)>1000) ||
      !is.numeric(temperature) || is.complex(temperature) ||
      !is.null(dim(temperature)) || length(temperature)!=1L ||
      !is.finite(temperature) || temperature<0.01 || temperature>100) {
    stop("invalid logits or temperature")
  }
  shifted <- (logits-max(logits))/temperature
  weights <- exp(shifted)
  as.double(weights/sum(weights))
}
