# Exact Rabin-Karp substring search with rolling hash and equality verification.
rabin_karp_substring_search <- function(text, pattern) {
  checked <- function(x) {
    if (!is.character(x) || length(x) != 1L || is.na(x)) stop("expected one ASCII string")
    codes <- utf8ToInt(enc2utf8(x))
    if (length(codes) > 512L || any(codes < 32L) || any(codes > 126L)) {
      stop("expected printable ASCII at most 512 characters")
    }
    codes
  }
  t <- checked(text)
  p <- checked(pattern)
  n <- length(t)
  m <- length(p)
  if (m == 0L) return(as.integer(0:n))
  if (m > n) return(integer(0))
  base <- 257
  modulus <- 1000000007
  # All products are exact integers below 2^53. Avoid R's floating
  # remainder operator, which may introduce fractional rounding errors.
  reduce <- function(x) x - floor(x / modulus) * modulus
  high_power <- 1
  if (m > 1L) for (i in seq_len(m - 1L)) high_power <- reduce(high_power * base)
  ph <- 0
  th <- 0
  for (i in seq_len(m)) {
    ph <- reduce(ph * base + p[[i]])
    th <- reduce(th * base + t[[i]])
  }
  result <- integer(n - m + 1L)
  count <- 0L
  for (i in seq_len(n - m + 1L)) {
    if (ph == th && identical(t[seq.int(i, i + m - 1L)], p)) {
      count <- count + 1L
      result[[count]] <- as.integer(i - 1L)
    }
    if (i <= n - m) {
      outgoing <- reduce(t[[i]] * high_power)
      th <- reduce(reduce(th + modulus - outgoing) * base + t[[i + m]])
    }
  }
  result[seq_len(count)]
}
