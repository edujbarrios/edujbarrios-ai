# Tversky index for exact signed 32-bit token sets.
tversky_index <- function(first, second, alpha = 1, beta = 1) {
  check_parameter <- function(x) {
    if (!is.numeric(x) || is.complex(x) || length(x) != 1L ||
        !is.finite(x) || x <= 0) {
      stop("finite strictly positive parameter required")
    }
    as.double(x)
  }
  check_ids <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        any(!is.finite(x)) || any(x != trunc(x)) ||
        any(x < -2147483648 | x > 2147483647)) {
      stop("signed 32-bit token IDs required")
    }
    sort(unique(as.double(x)))
  }
  alpha <- check_parameter(alpha)
  beta <- check_parameter(beta)
  a <- check_ids(first)
  b <- check_ids(second)
  if (length(a) == 0L && length(b) == 0L) return(1.0)
  if (length(a) == 0L || length(b) == 0L) return(0.0)
  i <- 1L
  j <- 1L
  common <- 0
  while (i <= length(a) && j <= length(b)) {
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
  denominator <- common + alpha * (length(a) - common) +
    beta * (length(b) - common)
  if (!is.finite(denominator) || denominator <= 0) {
    stop("unrepresentable denominator")
  }
  min(1.0, max(0.0, common / denominator))
}
