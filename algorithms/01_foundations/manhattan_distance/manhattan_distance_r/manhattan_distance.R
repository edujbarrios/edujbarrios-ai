# L1 distance of finite real vectors.
manhattan_distance <- function(left, right) {
  if (!is.numeric(left) || !is.numeric(right) ||
      is.complex(left) || is.complex(right)) stop("real vectors required")
  if (length(left) != length(right)) stop("length mismatch")
  if (any(!is.finite(left)) || any(!is.finite(right))) stop("non-finite coordinate")
  differences <- as.double(left) - as.double(right)
  if (any(!is.finite(differences))) stop("subtraction overflow")
  result <- sum(abs(differences))
  if (!is.finite(result)) stop("distance overflow")
  as.double(result)
}
