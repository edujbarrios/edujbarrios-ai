# Exact Dice similarity on unique, signed 32-bit integer IDs.
dice_similarity <- function(first, second) {
  validate <- function(x) {
    if (!is.numeric(x) || is.complex(x) || any(!is.finite(x)) ||
        any(x != trunc(x)) ||
        any(x < -2147483648 | x > 2147483647)) {
      stop("finite signed 32-bit whole-number IDs required")
    }
    sort(unique(as.double(x)))
  }
  a <- validate(first)
  b <- validate(second)
  na <- length(a)
  nb <- length(b)
  if (na + nb == 0) return(1.0)
  i <- 1L
  j <- 1L
  overlap <- 0
  while (i <= na && j <= nb) {
    if (a[[i]] == b[[j]]) {
      overlap <- overlap + 1
      i <- i + 1L
      j <- j + 1L
    } else if (a[[i]] < b[[j]]) {
      i <- i + 1L
    } else {
      j <- j + 1L
    }
  }
  2.0 * overlap / (na + nb)
}
