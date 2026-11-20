# Empirical mutual information (bits) from aligned binary observations.
binary_mutual_information <- function(x, y) {
  if (!is.numeric(x) || is.complex(x) ||
      !is.numeric(y) || is.complex(y) ||
      length(x) == 0L || length(x) != length(y) ||
      any(!is.finite(x)) || any(!is.finite(y)) ||
      any(!(x %in% c(0, 1))) || any(!(y %in% c(0, 1)))) {
    stop("nonempty aligned binary observations required")
  }
  counts <- matrix(0, nrow=2L, ncol=2L)
  for (i in seq_along(x)) counts[x[[i]] + 1L, y[[i]] + 1L] <-
    counts[x[[i]] + 1L, y[[i]] + 1L] + 1
  row_counts <- rowSums(counts)
  col_counts <- colSums(counts)
  n <- length(x)
  result <- 0.0
  for (a in 1:2) {
    for (b in 1:2) {
      if (counts[a,b] > 0) {
        p <- counts[a,b] / n
        result <- result + p * log2(p / ((row_counts[[a]]/n) * (col_counts[[b]]/n)))
      }
    }
  }
  if (!is.finite(result)) stop("nonfinite mutual information")
  max(0.0, result)
}
