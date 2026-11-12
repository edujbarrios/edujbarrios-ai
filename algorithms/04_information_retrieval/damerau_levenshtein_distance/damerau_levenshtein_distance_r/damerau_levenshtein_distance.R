# Full Damerau edit distance with a last-seen table for byte token IDs.
damerau_levenshtein_distance <- function(first, second) {
  validate <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        length(x) > 256L || any(!is.finite(x)) ||
        any(x != trunc(x)) || any(x < 0 | x > 255)) {
      stop("0..256 byte token IDs in one numeric vector required")
    }
    as.integer(x)
  }
  a <- validate(first)
  b <- validate(second)
  n <- length(a)
  m <- length(b)
  upper <- n + m
  h <- matrix(0, nrow=n + 2L, ncol=m + 2L)
  h[1L, 1L] <- upper
  for (i in 0:n) {
    h[i + 2L, 1L] <- upper
    h[i + 2L, 2L] <- i
  }
  for (j in 0:m) {
    h[1L, j + 2L] <- upper
    h[2L, j + 2L] <- j
  }
  seen <- integer(256L)
  for (i in seq_len(n)) {
    last_match <- 0L
    for (j in seq_len(m)) {
      prev_a <- seen[[b[[j]] + 1L]]
      prev_b <- last_match
      cost <- if (a[[i]] == b[[j]]) 0L else 1L
      if (cost == 0L) last_match <- j
      h[i + 2L, j + 2L] <- min(
        h[i + 1L, j + 1L] + cost,
        h[i + 2L, j + 1L] + 1,
        h[i + 1L, j + 2L] + 1,
        h[prev_a + 1L, prev_b + 1L] + (i - prev_a - 1L) +
          1L + (j - prev_b - 1L)
      )
    }
    seen[[a[[i]] + 1L]] <- i
  }
  h[n + 2L, m + 2L]
}
