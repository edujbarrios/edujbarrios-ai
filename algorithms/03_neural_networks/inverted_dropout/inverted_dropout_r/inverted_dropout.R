# Inverted dropout forward pass, with explicit binary mask.
inverted_dropout <- function(activations, mask, probability) {
  if (!is.numeric(activations) || is.complex(activations) ||
      !is.numeric(mask) || is.complex(mask) ||
      !is.numeric(probability) || length(probability) != 1L ||
      is.complex(probability) || !is.finite(probability) ||
      probability < 0 || probability >= 1 ||
      length(activations) != length(mask) ||
      any(!is.finite(activations)) || any(!is.finite(mask)) ||
      any(!(mask %in% c(0, 1)))) {
    stop("aligned finite activations, binary mask and p in [0,1) required")
  }
  result <- numeric(length(activations))
  selected <- which(mask == 1)
  result[selected] <- as.double(activations[selected]) / (1 - probability)
  if (any(!is.finite(result))) stop("retained activation scaling overflow")
  result
}
