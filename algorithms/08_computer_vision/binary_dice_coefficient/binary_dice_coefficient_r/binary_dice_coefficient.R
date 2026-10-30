# Binary foreground Dice overlap with both-all-zero convention of 1.
binary_dice_coefficient <- function(prediction, target) {
  valid <- function(x) {
    is.numeric(x) && !is.complex(x) && all(is.finite(x)) && all(x == 0 | x == 1)
  }
  if (length(prediction) == 0L || length(prediction) != length(target) ||
      !valid(prediction) || !valid(target)) {
    stop("masks must be nonempty equal-sized binary numeric vectors")
  }
  overlap <- sum(prediction == 1 & target == 1)
  total <- sum(prediction) + sum(target)
  if (total == 0) 1.0 else 2 * overlap / total
}
