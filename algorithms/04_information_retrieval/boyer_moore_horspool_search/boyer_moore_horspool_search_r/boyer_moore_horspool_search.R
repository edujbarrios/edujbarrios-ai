# Exact printable ASCII Boyer-Moore-Horspool substring matching.
boyer_moore_horspool_search <- function(text, pattern) {
  checked <- function(x) {
    if (!is.character(x) || length(x)!=1L || is.na(x)) {
      stop("expected one printable ASCII string")
    }
    codes <- utf8ToInt(enc2utf8(x))
    if (length(codes)>512L || any(codes<32L) || any(codes>126L)) {
      stop("expected printable ASCII of length at most 512")
    }
    codes
  }
  t <- checked(text)
  p <- checked(pattern)
  n <- length(t)
  m <- length(p)
  if (m==0L) return(as.integer(0:n))
  if (m>n) return(integer(0))
  shift <- rep.int(as.integer(m),128L)
  if (m>1L) {
    for (j in seq_len(m-1L)) {
      shift[[p[[j]]+1L]] <- as.integer(m-j)
    }
  }
  result <- integer(n-m+1L)
  count <- 0L
  start <- 0L
  while (start<=n-m) {
    j <- as.integer(m)
    while (j>=1L && t[[start+j]]==p[[j]]) j <- j-1L
    if (j==0L) {
      count <- count+1L
      result[[count]] <- as.integer(start)
    }
    start <- start+shift[[t[[start+m]]+1L]]
  }
  result[seq_len(count)]
}
