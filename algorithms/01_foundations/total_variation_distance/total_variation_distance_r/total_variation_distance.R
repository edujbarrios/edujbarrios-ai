# Total variation distance between finite discrete probability vectors.
total_variation_distance <- function(first, second) {
  validate <- function(x) {
    if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
        length(x) == 0L || any(!is.finite(x)) ||
        any(x < 0 | x > 1) || abs(sum(x) - 1) > 1e-9) {
      stop("nonempty finite probability vector summing to one required")
    }
    as.double(x)
  }
  p <- validate(first)
  q <- validate(second)
  if (length(p) != length(q)) stop("distribution sizes differ")
  min(1.0, max(0.0, 0.5 * sum(abs(p - q))))
}
