# Two-class Laplace-smoothed Bernoulli naive Bayes for a flat row-major matrix.
bernoulli_naive_bayes <- function(samples, labels, n, d, query) {
  scalar_dim <- function(x, lo, hi) {
    is.numeric(x) && length(x) == 1L && is.finite(x) &&
      x == trunc(x) && x >= lo && x <= hi
  }
  binary <- function(x, count) {
    is.numeric(x) && is.null(dim(x)) && length(x) == count &&
      all(is.finite(x)) && all(x %in% c(0, 1))
  }
  if (!scalar_dim(n, 2, 128) || !scalar_dim(d, 1, 16)) {
    stop("n in 2..128 and d in 1..16 required")
  }
  n <- as.integer(n)
  d <- as.integer(d)
  if (!binary(samples, n * d) || !binary(labels, n) ||
      !binary(query, d)) {
    stop("binary integer vectors with exact lengths required")
  }
  counts <- c(sum(labels == 0), sum(labels == 1))
  if (any(counts == 0)) stop("both classes must occur")
  ones <- matrix(0, nrow=2, ncol=d)
  for (i in seq_len(n)) {
    c <- as.integer(labels[[i]]) + 1L
    for (j in seq_len(d)) {
      ones[c, j] <- ones[c, j] + samples[[(i - 1L) * d + j]]
    }
  }
  scores <- log(counts / n)
  for (c in 1:2) {
    for (j in seq_len(d)) {
      p <- (ones[c, j] + 1) / (counts[[c]] + 2)
      scores[[c]] <- scores[[c]] +
        if (query[[j]] == 1) log(p) else log1p(-p)
    }
  }
  delta <- scores[[1]] - scores[[2]]
  posterior <- if (delta >= 0) exp(-delta) / (1 + exp(-delta)) else
    1 / (1 + exp(delta))
  list(label=if (scores[[2]] > scores[[1]]) 1L else 0L,
       probability_1=posterior)
}
