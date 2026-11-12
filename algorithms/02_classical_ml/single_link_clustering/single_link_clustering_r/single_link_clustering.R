# Deterministic single-link clustering of flattened 2D integer coordinates.
single_link_clustering <- function(points, n, k) {
  scalar_integer <- function(x, low, high) {
    is.numeric(x) && length(x) == 1L && is.finite(x) &&
      x == trunc(x) && x >= low && x <= high
  }
  if (!scalar_integer(n, 1, 32) || !scalar_integer(k, 1, n)) {
    stop("n must be 1..32 and k must be 1..n")
  }
  n <- as.integer(n)
  k <- as.integer(k)
  if (!is.numeric(points) || is.complex(points) || !is.null(dim(points)) ||
      length(points) != 2L*n || any(!is.finite(points)) ||
      any(points != trunc(points)) || any(abs(points) > 1000)) {
    stop("flat bounded integer coordinate vector of length 2*n required")
  }
  groups <- seq.int(0L, n - 1L)
  for (step in seq_len(n - k)) {
    best_dist <- Inf
    best_i <- 0L
    best_j <- 0L
    for (i in seq_len(n)) {
      if (i >= n) next
      for (j in seq.int(i + 1L, n)) {
        if (groups[[i]] == groups[[j]]) next
        dx <- points[[2L*i - 1L]] - points[[2L*j - 1L]]
        dy <- points[[2L*i]] - points[[2L*j]]
        dist <- dx*dx + dy*dy
        if (dist < best_dist) {
          best_dist <- dist
          best_i <- i
          best_j <- j
        }
      }
    }
    old <- groups[[best_j]]
    new <- groups[[best_i]]
    groups[groups == old] <- new
  }
  seen <- rep(NA_integer_, n)
  result <- integer(n)
  next_id <- 0L
  for (i in seq_len(n)) {
    group <- groups[[i]] + 1L
    if (is.na(seen[[group]])) {
      seen[[group]] <- next_id
      next_id <- next_id + 1L
    }
    result[[i]] <- seen[[group]]
  }
  result
}
