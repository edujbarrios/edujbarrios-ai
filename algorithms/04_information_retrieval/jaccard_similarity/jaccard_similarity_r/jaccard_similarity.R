# Exact Jaccard set similarity of signed 32-bit integer token IDs.
jaccard_similarity <- function(left, right) {
  validate <- function(x) {
    if (!is.numeric(x) || is.complex(x) || any(!is.finite(x)) ||
        any(x != floor(x)) || any(x < -2147483648) ||
        any(x > 2147483647)) {
      stop("token IDs must be finite signed 32-bit integers")
    }
    sort(unique(as.double(x)))
  }
  a <- validate(left)
  b <- validate(right)
  if (length(a) == 0L && length(b) == 0L) return(1.0)
  i <- 1L
  j <- 1L
  shared <- 0.0
  union_count <- 0.0
  while (i <= length(a) || j <= length(b)) {
    if (j > length(b) || (i <= length(a) && a[[i]] < b[[j]])) {
      i <- i + 1L
    } else if (i > length(a) || b[[j]] < a[[i]]) {
      j <- j + 1L
    } else {
      shared <- shared + 1
      i <- i + 1L
      j <- j + 1L
    }
    union_count <- union_count + 1
  }
  as.double(shared / union_count)
}
