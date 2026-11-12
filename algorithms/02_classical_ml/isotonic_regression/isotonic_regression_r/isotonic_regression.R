# Equal-weight isotonic regression with pool-adjacent-violators.
isotonic_regression <- function(values) {
  n <- length(values)
  if (!is.numeric(values) || is.complex(values) || !is.null(dim(values)) ||
      n < 1L || n > 512L || any(!is.finite(values)) ||
      any(abs(values) > 1000000)) {
    stop("1..512 bounded finite numeric observations required")
  }
  means <- numeric(n)
  counts <- integer(n)
  blocks <- 0L
  for (value in values) {
    blocks <- blocks + 1L
    means[[blocks]] <- as.double(value)
    counts[[blocks]] <- 1L
    while (blocks > 1L && means[[blocks - 1L]] > means[[blocks]]) {
      count <- counts[[blocks - 1L]] + counts[[blocks]]
      means[[blocks - 1L]] <-
        (means[[blocks - 1L]] * counts[[blocks - 1L]] +
         means[[blocks]] * counts[[blocks]]) / count
      counts[[blocks - 1L]] <- count
      blocks <- blocks - 1L
    }
  }
  result <- numeric(n)
  start <- 1L
  for (k in seq_len(blocks)) {
    end <- start + counts[[k]] - 1L
    result[start:end] <- means[[k]]
    start <- end + 1L
  }
  result
}
