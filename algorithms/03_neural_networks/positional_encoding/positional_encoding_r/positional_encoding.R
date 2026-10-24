# Deterministic sine/cosine Transformer encodings for zero-based positions.
positional_encoding <- function(length, width) {
  valid_count <- function(value, upper) {
    is.numeric(value) && !is.complex(value) && length(value)==1L &&
      is.finite(value) && value==floor(value) && value>=1 && value<=upper
  }
  if (!valid_count(length,4096) || !valid_count(width,1024)) {
    stop("length must be 1..4096 and width 1..1024")
  }
  n <- as.integer(length)
  d <- as.integer(width)
  output <- matrix(0.0,nrow=n,ncol=d)
  for (p in seq_len(n)) {
    for (j in seq_len(d)) {
      pair <- (j-1L)%/%2L
      angle <- (p-1L)/(10000.0^(2.0*pair/d))
      output[p,j] <- if ((j-1L)%%2L==0L) sin(angle) else cos(angle)
    }
  }
  output
}
