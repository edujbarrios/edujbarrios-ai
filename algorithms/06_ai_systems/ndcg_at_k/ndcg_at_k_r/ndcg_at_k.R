# NDCG@K for integer grades between zero and twenty.
ndcg_at_k <- function(grades, k) {
  if (!is.numeric(grades) || is.complex(grades) || length(grades) == 0L ||
      any(!is.finite(grades)) || any(grades < 0) || any(grades > 20) ||
      any(grades != floor(grades))) {
    stop("grades must be integer values in 0..20")
  }
  if (!is.numeric(k) || is.complex(k) || length(k) != 1L ||
      !is.finite(k) || k != floor(k) || k < 1 || k > length(grades)) {
    stop("k must be in 1..length(grades)")
  }
  dcg <- function(items) {
    total <- 0.0
    for (i in seq_len(k)) {
      total <- total + ((2^items[[i]]) - 1) / log2(i + 1)
    }
    total
  }
  current <- dcg(grades)
  ideal <- dcg(sort(grades, decreasing = TRUE))
  if (!is.finite(current) || !is.finite(ideal)) stop("non-finite gain")
  if (ideal == 0) return(0.0)
  max(0.0, min(1.0, current / ideal))
}
