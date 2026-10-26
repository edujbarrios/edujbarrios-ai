# Symmetric Hellinger distance between discrete distributions.
hellinger_distance <- function(first, second) {
  validate <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        length(x) == 0L || any(!is.finite(x)) ||
        any(x < 0 | x > 1) || abs(sum(x) - 1) > 1e-9) {
      stop("nonempty probability vector summing to one required")
    }
    as.numeric(x)
  }
  p <- validate(first)
  q <- validate(second)
  if (length(p) != length(q)) stop("probability vector lengths differ")
  value <- sqrt(sum((sqrt(p) - sqrt(q))^2) / 2)
  min(1.0, max(0.0, value))
}
