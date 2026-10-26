# Zero-safe and overflow-resistant Canberra distance for real vectors.
canberra_distance <- function(first, second) {
  check <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        length(x) == 0L || any(!is.finite(x))) {
      stop("nonempty finite real vector required")
    }
    as.double(x)
  }
  a <- check(first)
  b <- check(second)
  if (length(a) != length(b)) stop("vector lengths differ")
  terms <- numeric(length(a))
  for (i in seq_along(a)) {
    scale <- max(abs(a[[i]]), abs(b[[i]]))
    if (scale == 0) next
    x <- a[[i]] / scale
    y <- b[[i]] / scale
    terms[[i]] <- abs(x - y) / (abs(x) + abs(y))
  }
  distance <- sum(terms)
  if (!is.finite(distance)) stop("nonfinite result")
  distance
}
