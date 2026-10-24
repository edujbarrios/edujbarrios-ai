# Exhaustive binary threshold classifier with deterministic ties.
decision_stump_predict <- function(feature, threshold, positive_right) {
  if (!is.numeric(feature) || is.complex(feature) ||
      length(feature)!=1L || !is.finite(feature) ||
      !is.numeric(threshold) || is.complex(threshold) ||
      length(threshold)!=1L || !is.finite(threshold) ||
      !is.logical(positive_right) || length(positive_right)!=1L ||
      is.na(positive_right)) {
    stop("expected finite scalars and a boolean orientation")
  }
  as.integer(if (positive_right) feature>threshold else feature<=threshold)
}
decision_stump <- function(x, labels) {
  if (!is.numeric(x) || is.complex(x) ||
      length(x)==0L || any(!is.finite(x)) ||
      !is.numeric(labels) || is.complex(labels) ||
      length(labels)!=length(x) || any(!is.finite(labels)) ||
      any(!(labels %in% c(0,1)))) {
    stop("nonempty finite features and binary labels required")
  }
  best_errors <- length(x)
  best_threshold <- as.double(x[[1L]])
  best_right <- TRUE
  for (threshold in x) {
    for (right in c(TRUE,FALSE)) {
      predictions <- if (right) x>threshold else x<=threshold
      errors <- sum(predictions!=(labels==1))
      if (errors<best_errors ||
          (errors==best_errors &&
            (threshold<best_threshold ||
             (threshold==best_threshold && right && !best_right)))) {
        best_errors <- errors
        best_threshold <- as.double(threshold)
        best_right <- right
      }
    }
  }
  list(threshold=best_threshold,positive_right=best_right,
       errors=as.integer(best_errors))
}
