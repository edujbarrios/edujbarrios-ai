# Chebyshev distance for finite real numeric vectors.
chebyshev_distance <- function(left, right) {
  if (!is.numeric(left) || !is.numeric(right) ||
      is.complex(left) || is.complex(right)) {
    stop("vectors must contain real numbers")
  }
  if (length(left) != length(right)) stop("length mismatch")
  if (any(!is.finite(left)) || any(!is.finite(right))) stop("non-finite coordinate")
  differences <- as.double(left) - as.double(right)
  if (any(!is.finite(differences))) stop("distance overflow")
  if (length(differences) == 0L) return(0.0)
  as.double(max(abs(differences)))
}
