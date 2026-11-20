# Prefix completion over a strictly ordered lowercase ASCII lexicon.
sorted_lexicon_prefix_search <- function(terms, prefix) {
  if (!is.character(terms) || !is.character(prefix) || length(prefix) != 1L ||
      is.na(prefix) || nchar(prefix, type = "bytes") > 512L ||
      !grepl("^[a-z]*$", prefix)) {
    stop("invalid dictionary or prefix")
  }
  if (anyNA(terms) ||
      any(nchar(terms, type = "bytes") > 512L) ||
      any(!grepl("^[a-z]+$", terms)) ||
      anyDuplicated(terms) > 0L ||
      !identical(terms, sort(terms, method = "radix"))) {
    stop("dictionary must be sorted unique lowercase ASCII words")
  }
  n <- length(terms)
  low <- 1L
  high <- n + 1L
  while (low < high) {
    mid <- low + (high - low) %/% 2L
    if (terms[[mid]] < prefix) low <- mid + 1L
    else high <- mid
  }
  result <- integer()
  while (low <= n && startsWith(terms[[low]], prefix)) {
    result <- c(result, low - 1L)
    low <- low + 1L
  }
  result
}
