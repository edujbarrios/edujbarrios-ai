# Longest contiguous common printable-ASCII substring with stable tie-breaks.
longest_common_substring <- function(a, b) {
  checked <- function(value) {
    if (!is.character(value) || length(value) != 1L || is.na(value)) {
      stop("expected one printable ASCII string")
    }
    codes <- utf8ToInt(enc2utf8(value))
    if (length(codes) > 512L || any(codes < 32L) || any(codes > 126L)) {
      stop("expected printable ASCII <=512 characters")
    }
    codes
  }
  x <- checked(a)
  y <- checked(b)
  m <- length(y)
  prev <- integer(m + 1L)
  best <- 0L
  start_a <- 0L
  start_b <- 0L
  for (i in seq_along(x)) {
    curr <- integer(m + 1L)
    for (j in seq_along(y)) {
      if (x[[i]] != y[[j]]) next
      len <- prev[[j]] + 1L
      curr[[j + 1L]] <- len
      sa <- as.integer(i - len)
      sb <- as.integer(j - len)
      if (len > best ||
          (len == best && len > 0L &&
           (sa < start_a || (sa == start_a && sb < start_b)))) {
        best <- len
        start_a <- sa
        start_b <- sb
      }
    }
    prev <- curr
  }
  list(length=best, start_a=start_a, start_b=start_b)
}
