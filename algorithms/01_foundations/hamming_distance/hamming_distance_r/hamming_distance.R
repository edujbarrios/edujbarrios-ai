# Symbol-wise Hamming distance for equally sized raw (byte) vectors.
hamming_distance <- function(a, b) {
  if (!is.raw(a) || !is.raw(b) || length(a) != length(b)) {
    stop("inputs must be equal-length raw vectors")
  }
  sum(a != b)
}
