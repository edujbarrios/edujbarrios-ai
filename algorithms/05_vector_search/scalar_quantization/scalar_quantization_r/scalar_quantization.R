# Uniform 8-bit quantization of finite numeric vectors.
scalar_quantization <- function(values, minimum, maximum) {
  if (!is.numeric(values) || is.complex(values) ||
      any(!is.finite(values)) ||
      !is.numeric(minimum) || is.complex(minimum) ||
      !is.numeric(maximum) || is.complex(maximum) ||
      length(minimum) != 1L || length(maximum) != 1L ||
      !is.finite(minimum) || !is.finite(maximum) ||
      minimum >= maximum) {
    stop("invalid values or calibration bounds")
  }
  span <- as.double(maximum) - as.double(minimum)
  if (!is.finite(span) || span <= 0) stop("range span must be finite")
  if (length(values) == 0L) return(integer())
  result <- integer(length(values))
  for (i in seq_along(values)) {
    x <- as.double(values[[i]])
    if (x <= minimum) result[[i]] <- 0L
    else if (x >= maximum) result[[i]] <- 255L
    else result[[i]] <- as.integer(min(255, floor(255 * ((x - minimum)/span) + 0.5)))
  }
  result
}
