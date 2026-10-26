# Longest common subsequence length for signed 32-bit integer token IDs.
longest_common_subsequence <- function(first, second) {
  validate <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        any(!is.finite(x)) || any(x != trunc(x)) ||
        any(x < -2147483648 | x > 2147483647)) {
      stop("signed 32-bit integer token vector required")
    }
    as.double(x)
  }
  a <- validate(first)
  b <- validate(second)
  if (length(a) < length(b)) {
    temporary <- a
    a <- b
    b <- temporary
  }
  if (length(b) == 0L) return(0)
  previous <- numeric(length(b) + 1L)
  current <- numeric(length(b) + 1L)
  for (item in a) {
    current[[1L]] <- 0
    for (j in seq_along(b)) {
      current[[j + 1L]] <- if (item == b[[j]]) {
        previous[[j]] + 1
      } else {
        max(previous[[j + 1L]], current[[j]])
      }
    }
    temporary <- previous
    previous <- current
    current <- temporary
  }
  previous[[length(previous)]]
}
