# Exact ASCII Z-algorithm substring search, all overlapping matches.
z_algorithm_substring_search <- function(text, pattern) {
  checked <- function(value) {
    if (!is.character(value) || length(value)!=1L || is.na(value)) {
      stop("expected one printable ASCII string")
    }
    codes <- utf8ToInt(enc2utf8(value))
    if (length(codes)>512L || any(codes<32L) || any(codes>126L)) {
      stop("expected printable ASCII string <=512 characters")
    }
    codes
  }
  t <- checked(text)
  p <- checked(pattern)
  n <- length(t)
  m <- length(p)
  if (m==0L) return(as.integer(0:n))
  if (m>n) return(integer(0))
  seq <- c(p,0L,t)
  total <- length(seq)
  z <- integer(total)
  left <- 0L
  right <- 0L
  matches <- integer(n-m+1L)
  count <- 0L
  for (i in seq.int(1L,total-1L)) {
    if (i<right) z[[i+1L]] <- min(right-i,z[[i-left+1L]])
    while (i+z[[i+1L]]<total &&
           seq[[z[[i+1L]]+1L]]==seq[[i+z[[i+1L]]+1L]]) {
      z[[i+1L]] <- z[[i+1L]]+1L
    }
    if (i+z[[i+1L]]>right) {
      left <- i
      right <- i+z[[i+1L]]
    }
    if (i>=m+1L && z[[i+1L]]>=m) {
      count <- count+1L
      matches[[count]] <- i-m-1L
    }
  }
  matches[seq_len(count)]
}
