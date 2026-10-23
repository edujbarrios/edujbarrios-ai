# Dot product of finite real vectors. Empty vectors return 0.0.
dot_product <- function(left, right) {
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

  # Base R vectorizes multiplication, allocating an O(n) intermediate vector.
  products <- as.double(left) * as.double(right)
  if (any(!is.finite(products))) {
    stop("dot product overflow")
  }
  result <- sum(products)
  if (!is.finite(result)) {
    stop("dot product overflow")
  }
  as.double(result)
}
