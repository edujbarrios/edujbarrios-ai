# Exact Hamming top-k over 30-bit packed integer signatures.
hamming_top_k <- function(query, candidates, k) {
  valid <- function(x) is.numeric(x) && !is.complex(x) &&
    all(is.finite(x)) && all(x == floor(x)) &&
    all(x >= 0) && all(x <= 2^30 - 1)
  if (!valid(query) || length(query) != 1L ||
      !valid(candidates) || length(candidates) == 0L ||
      !valid(k) || length(k) != 1L || k < 1 ||
      k > length(candidates)) {
    stop("expected 30-bit integer signatures and k in 1..n")
  }
  distances <- integer(length(candidates))
  for (i in seq_along(candidates)) {
    bits <- bitwXor(as.integer(query), as.integer(candidates[[i]]))
    count <- 0L
    for (bit in seq_len(30)) {
      count <- count + bitwAnd(bits, 1L)
      bits <- bitwShiftR(bits, 1L)
    }
    distances[[i]] <- count
  }
  as.integer(order(distances, seq_along(candidates))[seq_len(k)] - 1L)
}
