# Contrastive loss for a labeled pair of Euclidean embeddings.
contrastive_pair_loss <- function(first, second, label, margin) {
  valid <- function(x) {
    is.numeric(x) && !is.complex(x) && is.null(dim(x)) &&
      all(is.finite(x)) && all(abs(x) <= 1000)
  }
  d <- length(first)
  if (!valid(first) || !valid(second) || d < 1L || d > 512L ||
      length(second) != d || !is.numeric(label) || length(label) != 1L ||
      !is.finite(label) || !(label %in% c(0,1)) ||
      !is.numeric(margin) || length(margin) != 1L ||
      !is.finite(margin) || margin < 0 || margin > 1000) {
    stop("two bounded finite vectors, binary label and valid margin required")
  }
  squared <- sum((first-second)^2)
  gap <- max(0, margin - sqrt(squared))
  0.5 * if (label == 1) squared else gap^2
}
