# Jaro-Winkler similarity for printable ASCII with fixed threshold and prefix boost.
jaro_winkler_similarity <- function(a, b) {
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
  x <- checked(a)
  y <- checked(b)
  n <- length(x)
  m <- length(y)
  if (n == 0L || m == 0L) return(if (n == m) 1.0 else 0.0)
  window <- max(0L, as.integer(max(n, m) %/% 2L - 1L))
  matched_x <- rep(FALSE, n)
  matched_y <- rep(FALSE, m)
  matches <- 0L
  for (i in seq_len(n)) {
    first <- max(1L, i - window)
    last <- min(m, i + window)
    if (first <= last) {
      for (j in seq.int(first, last)) {
        if (!matched_y[[j]] && x[[i]] == y[[j]]) {
          matched_x[[i]] <- TRUE
          matched_y[[j]] <- TRUE
          matches <- matches + 1L
          break
        }
      }
    }
  }
  if (matches == 0L) return(0.0)
  mismatches <- 0L
  j <- 1L
  for (i in seq_len(n)) {
    if (!matched_x[[i]]) next
    while (!matched_y[[j]]) j <- j + 1L
    if (x[[i]] != y[[j]]) mismatches <- mismatches + 1L
    j <- j + 1L
  }
  matched <- as.numeric(matches)
  jaro <- (matched / n + matched / m +
           (matched - mismatches / 2) / matched) / 3
  prefix <- 0L
  for (i in seq_len(min(4L, n, m))) {
    if (x[[i]] != y[[i]]) break
    prefix <- prefix + 1L
  }
  if (jaro > 0.7) jaro + 0.1 * prefix * (1 - jaro) else jaro
}
