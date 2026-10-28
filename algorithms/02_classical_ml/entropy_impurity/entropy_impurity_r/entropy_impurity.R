# Classification entropy in bits for bounded integer-valued counts.
entropy_impurity <- function(counts) {
  if (!is.numeric(counts) || is.complex(counts) ||
      !is.null(dim(counts)) || length(counts)<1L ||
      length(counts)>128L || any(!is.finite(counts)) ||
      any(counts<0) || any(counts>1000000) ||
      any(counts!=floor(counts))) {
    stop("invalid class counts")
  }
  total <- sum(counts)
  if (total==0) stop("at least one positive count required")
  probabilities <- counts[counts>0]/total
  as.double(max(0,-sum(probabilities*log2(probabilities))))
}
