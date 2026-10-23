# Stable Euclidean distance for finite numeric vectors.
euclidean_distance <- function(left, right) {
  if (!is.numeric(left) || !is.numeric(right) ||
      is.complex(left) || is.complex(right)) {
    stop("vectors must contain real numbers")
  }
  if (length(left) != length(right)) {
    stop("vectors must have equal lengths")
  }
  if (any(!is.finite(left)) || any(!is.finite(right))) {
    stop("values must be finite")
  }

  differences <- as.double(left) - as.double(right)
  if (any(!is.finite(differences))) {
    stop("distance overflow")
  }

  # Scaled sum of squares avoids overflow from naive squaring.
  scale <- 0.0
  sum_sq <- 1.0
  for (difference in differences) {
    absolute <- abs(difference)
    if (absolute > 0) {
      if (scale < absolute) {
        sum_sq <- 1.0 + sum_sq * (scale / absolute)^2
        scale <- absolute
      } else {
        sum_sq <- sum_sq + (absolute / scale)^2
      }
    }
  }
  result <- if (scale == 0) 0.0 else scale * sqrt(sum_sq)
  if (!is.finite(result)) {
    stop("distance overflow")
  }
  as.double(result)
}
