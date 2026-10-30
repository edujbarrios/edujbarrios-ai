# Element-wise gradient clipping to a positive symmetric bound.
gradient_clip_by_value <- function(gradients, clip_value) {
  if (!is.numeric(gradients) || is.complex(gradients) ||
      length(gradients)==0L || any(!is.finite(gradients)) ||
      !is.numeric(clip_value) || is.complex(clip_value) ||
      length(clip_value)!=1L || !is.finite(clip_value) || clip_value<=0) {
    stop("gradients must be finite and nonempty; clipping limit positive")
  }
  pmin(as.double(clip_value), pmax(-as.double(clip_value), as.double(gradients)))
}
