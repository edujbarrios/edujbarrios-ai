# Exact overlap coefficient for signed 32-bit integer token IDs.
overlap_coefficient <- function(first, second) {
  checked <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        any(!is.finite(x)) || any(x != trunc(x)) ||
        any(x < -2147483648 | x > 2147483647)) {
      stop("signed 32-bit integer token IDs required")
    }
    sort(unique(as.double(x)))
  }
  a <- checked(first)
  b <- checked(second)
  na <- length(a)
  nb <- length(b)
  if (na == 0L && nb == 0L) return(1.0)
  if (na == 0L || nb == 0L) return(0.0)
  i <- 1L
  j <- 1L
  common <- 0
  while (i <= na && j <= nb) {
    if (a[[i]] == b[[j]]) {
      common <- common + 1
      i <- i + 1L
      j <- j + 1L
    } else if (a[[i]] < b[[j]]) {
      i <- i + 1L
    } else {
      j <- j + 1L
    }
  }
  common / min(na, nb)
}
