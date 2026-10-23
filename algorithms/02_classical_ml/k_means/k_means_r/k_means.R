# Deterministic, dense Lloyd K-means using first-k rows as initial centers.
k_means <- function(samples, k, max_iterations) {
  if (!is.matrix(samples) || !is.numeric(samples) ||
      is.complex(samples) || nrow(samples) < 1L || ncol(samples) < 1L ||
      any(!is.finite(samples))) {
    stop("samples must be a nonempty finite real matrix")
  }
  n <- nrow(samples)
  d <- ncol(samples)
  if (!is.numeric(k) || length(k) != 1L || !is.finite(k) ||
      k != floor(k) || k < 1 || k > n ||
      !is.numeric(max_iterations) || length(max_iterations) != 1L ||
      !is.finite(max_iterations) || max_iterations != floor(max_iterations) ||
      max_iterations < 1) {
    stop("invalid k or max_iterations")
  }
  k <- as.integer(k)
  max_iterations <- as.integer(max_iterations)
  centers <- matrix(as.double(samples[seq_len(k), , drop = FALSE]), nrow = k, ncol = d)
  if (k > 1L) {
    for (i in 2:k) {
      for (j in seq_len(i-1L)) {
        if (all(centers[i, ] == centers[j, ])) {
          stop("first k samples must be pairwise distinct")
        }
      }
    }
  }
  labels <- rep(-1L, n)
  for (iteration in seq_len(max_iterations)) {
    next_labels <- integer(n)
    sums <- matrix(0.0, nrow = k, ncol = d)
    counts <- integer(k)
    for (i in seq_len(n)) {
      best_distance <- Inf
      best_index <- 1L
      for (cluster in seq_len(k)) {
        scale <- 0.0
        ssq <- 1.0
        for (col in seq_len(d)) {
          delta <- as.double(samples[i, col]) - centers[cluster, col]
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
        distance <- if (scale == 0) 0.0 else scale * sqrt(ssq)
        if (!is.finite(distance)) stop("distance overflow")
        if (distance < best_distance) {
          best_distance <- distance
          best_index <- cluster
        }
      }
      next_labels[[i]] <- best_index - 1L
      counts[[best_index]] <- counts[[best_index]] + 1L
      for (col in seq_len(d)) {
        sums[best_index, col] <- sums[best_index, col] + as.double(samples[i, col])
        if (!is.finite(sums[best_index, col])) stop("centroid sum overflow")
      }
    }
    for (cluster in seq_len(k)) {
      if (counts[[cluster]] > 0L) {
        for (col in seq_len(d)) {
          centers[cluster, col] <- sums[cluster, col] / counts[[cluster]]
          if (!is.finite(centers[cluster, col])) stop("centroid overflow")
        }
      }
    }
    unchanged <- identical(next_labels, labels)
    labels <- next_labels
    if (unchanged) break
  }
  list(centroids = centers, assignments = labels)
}
