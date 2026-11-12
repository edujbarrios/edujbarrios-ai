# Smoothed soft Dice loss for binary segmentation, epsilon fixed at 1.
soft_dice_loss <- function(predictions, targets) {
  if (!is.numeric(predictions) || !is.numeric(targets) ||
      is.complex(predictions) || is.complex(targets) ||
      !is.null(dim(predictions)) || !is.null(dim(targets)) ||
      length(predictions) < 1L || length(predictions) > 512L ||
      length(predictions) != length(targets) ||
      any(!is.finite(predictions)) || any(predictions < 0 | predictions > 1) ||
      any(!is.finite(targets)) || any(!(targets %in% c(0,1)))) {
    stop("1..512 aligned predictions in [0,1] and binary integer targets required")
  }
  overlap <- sum(predictions * targets)
  1 - (2 * overlap + 1) / (sum(predictions) + sum(targets) + 1)
}
