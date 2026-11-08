# Exact KMP substring search on printable ASCII; return zero-based offsets.
kmp_substring_search <- function(text, pattern) {
  checked <- function(value) {
    if (!is.character(value) || length(value) != 1L || is.na(value)) {
      stop("expected one printable ASCII string")
    }
    codes <- utf8ToInt(enc2utf8(value))
    if (length(codes) > 512L || any(codes < 32L) || any(codes > 126L)) {
      stop("expected printable ASCII of length at most 512")
    }
    codes
  }
  t <- checked(text)
  p <- checked(pattern)
  n <- length(t)
  m <- length(p)
  if (m == 0L) return(as.integer(0:n))
  prefix <- integer(m)
  j <- 0L
  if (m >= 2L) {
    for (i in seq.int(2L, m)) {
      while (j > 0L && p[[i]] != p[[j + 1L]]) j <- prefix[[j]]
      if (p[[i]] == p[[j + 1L]]) j <- j + 1L
      prefix[[i]] <- j
    }
  }
  result <- integer(n + 1L)
  count <- 0L
  j <- 0L
  for (i in seq_along(t)) {
    while (j > 0L && t[[i]] != p[[j + 1L]]) j <- prefix[[j]]
    if (t[[i]] == p[[j + 1L]]) j <- j + 1L
    if (j == m) {
      count <- count + 1L
      result[[count]] <- as.integer(i - m)
      j <- prefix[[j]]
    }
  }
  result[seq_len(count)]
}
