# Stable global L2 gradient norm clipping.
gradient_clip_global_norm <- function(gradients, max_norm) {
  if (!is.numeric(gradients) || is.complex(gradients) ||
      length(gradients) == 0L || any(!is.finite(gradients)) ||
      !is.numeric(max_norm) || is.complex(max_norm) ||
      length(max_norm) != 1L || !is.finite(max_norm) || max_norm <= 0) {
    stop("gradients must be nonempty and finite; max_norm finite and positive")
  }
  values <- as.double(gradients)
  maximum <- max(abs(values))
  if (maximum == 0) return(values)
  scaled_norm <- sqrt(sum((values / maximum)^2))
  factor <- min(1, (as.double(max_norm) / maximum) / scaled_norm)
  values * factor
}
