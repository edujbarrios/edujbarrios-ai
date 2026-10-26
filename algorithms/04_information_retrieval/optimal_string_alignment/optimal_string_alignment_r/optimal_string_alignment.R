# Restricted Damerau edit distance using three dynamic-programming rows.
optimal_string_alignment <- function(first, second) {
  validate <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        any(!is.finite(x)) || any(x != trunc(x)) ||
        any(x < -2147483648 | x > 2147483647)) {
      stop("finite signed 32-bit integer token IDs required")
    }
    as.double(x)
  }
  a <- validate(first)
  b <- validate(second)
  if (length(a) < length(b)) {
    tmp <- a
    a <- b
    b <- tmp
  }
  n <- length(a)
  m <- length(b)
  if (m == 0L) return(as.double(n))
  older <- numeric(m + 1L)
  previous <- as.double(0:m)
  current <- numeric(m + 1L)
  for (i in seq_len(n)) {
    current[[1L]] <- i
    for (j in seq_len(m)) {
      cost <- if (a[[i]] == b[[j]]) 0 else 1
      best <- min(previous[[j + 1L]] + 1,
                  current[[j]] + 1, previous[[j]] + cost)
      if (i > 1L && j > 1L && a[[i]] == b[[j - 1L]] &&
          a[[i - 1L]] == b[[j]]) {
        best <- min(best, older[[j - 1L]] + 1)
      }
      current[[j + 1L]] <- best
    }
    tmp <- older
    older <- previous
    previous <- current
    current <- tmp
  }
  previous[[m + 1L]]
}
