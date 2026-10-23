# Rank fusion of exactly two rankings of zero-based nonnegative integer IDs.
reciprocal_rank_fusion <- function(a, b) {
  validate <- function(values) {
    if (!is.numeric(values) || is.complex(values) ||
        any(!is.finite(values)) ||
        any(values < 0 | values > 2147483647) ||
        any(values != floor(values)) || anyDuplicated(values) > 0L) {
      stop("rankings must have unique nonnegative integer IDs")
    }
  }
  validate(a)
  validate(b)
  if (length(a)==0L && length(b)==0L) stop("rankings cannot both be empty")
  items <- unique(c(as.integer(a), as.integer(b)))
  scores <- numeric(length(items))
  for (i in seq_along(items)) {
    at_a <- match(items[[i]], a)
    at_b <- match(items[[i]], b)
    if (!is.na(at_a)) scores[[i]] <- scores[[i]] + 1/(60+at_a)
    if (!is.na(at_b)) scores[[i]] <- scores[[i]] + 1/(60+at_b)
  }
  as.integer(items[order(-scores, items)])
}
