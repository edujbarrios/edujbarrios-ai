# Exact Euclidean KNN, returning zero-based indices in distance order.
knn <- function(samples, query, k) {
  if (!is.matrix(samples) || !is.numeric(samples) || is.complex(samples) ||
      !is.numeric(query) || is.complex(query) ||
      nrow(samples) < 1L || ncol(samples) < 1L ||
      length(query) != ncol(samples) || any(!is.finite(samples)) ||
      any(!is.finite(query))) {
    stop("samples must be finite numeric matrix and query must match")
  }
  if (!is.numeric(k) || length(k) != 1L || !is.finite(k) ||
      k != floor(k) || k < 1L || k > nrow(samples)) {
    stop("k must be integer in 1..nrow(samples)")
  }
  distances <- numeric(nrow(samples))
  for (i in seq_len(nrow(samples))) {
    scale <- 0.0
    ssq <- 1.0
    for (j in seq_len(ncol(samples))) {
      delta <- as.double(samples[i, j]) - as.double(query[[j]])
      if (!is.finite(delta)) stop("distance overflow")
      absolute <- abs(delta)
      if (absolute > 0) {
        if (scale < absolute) {
          ssq <- 1 + ssq * (scale / absolute)^2
          scale <- absolute
        } else {
          ssq <- ssq + (absolute / scale)^2
        }
      }
    }
    distances[[i]] <- if (scale == 0) 0.0 else scale * sqrt(ssq)
    if (!is.finite(distances[[i]])) stop("distance overflow")
  }
  as.integer(order(distances, seq_along(distances))[seq_len(k)] - 1L)
}
