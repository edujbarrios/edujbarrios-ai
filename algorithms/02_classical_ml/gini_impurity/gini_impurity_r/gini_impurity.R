# Gini impurity over integer-valued, nonnegative class counts.
gini_impurity <- function(counts) {
  if (!is.numeric(counts) || is.complex(counts) || !is.null(dim(counts)) ||
      length(counts)<1L || length(counts)>128L ||
      any(!is.finite(counts)) || any(counts<0) ||
      any(counts>1000000) || any(counts!=floor(counts))) {
    stop("invalid bounded nonnegative integer class counts")
  }
  total <- sum(counts)
  if (total==0) stop("at least one positive count is required")
  p <- as.double(counts)/total
  as.double(max(0,min(1,1-sum(p*p))))
}
