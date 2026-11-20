# Deterministic MinHash-style signature using modular integer hashes.
minhash_signature <- function(tokens, a, b) {
  prime <- 65521
  valid <- function(v, low, high) {
    is.numeric(v) && !is.complex(v) &&
      all(is.finite(v)) && all(v == floor(v)) &&
      all(v >= low & v <= high)
  }
  if (!valid(tokens, 0, prime-1) ||
      !valid(a, 1, prime-1) || !valid(b, 0, prime-1) ||
      length(a) == 0L || length(a) != length(b)) {
    stop("invalid tokens or hash coefficients")
  }
  result <- rep(as.double(prime), length(a))
  for (token in tokens) {
    for (j in seq_along(a)) {
      hashed <- (as.double(a[[j]]) * token + b[[j]]) %% prime
      result[[j]] <- min(result[[j]], hashed)
    }
  }
  result
}
