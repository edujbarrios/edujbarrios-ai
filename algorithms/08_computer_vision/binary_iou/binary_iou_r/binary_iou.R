# Binary mask intersection over union with explicit all-background convention.
binary_iou <- function(prediction, target) {
  valid <- function(mask) {
    is.numeric(mask) && !is.complex(mask) && all(is.finite(mask)) &&
      all(mask == 0 | mask == 1)
  }
  if (length(prediction) == 0L || length(prediction) != length(target) ||
      !valid(prediction) || !valid(target)) {
    stop("masks must be nonempty equal-length binary numeric vectors")
  }
  intersection <- sum(prediction == 1 & target == 1)
  union <- sum(prediction == 1 | target == 1)
  if (union == 0) 1.0 else intersection / union
}
