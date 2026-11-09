# Multiclass macro F1: average per-class F1 with absent-class score zero.
macro_f1_score <- function(actual, predicted, classes) {
  valid_classes <- is.numeric(classes) && !is.logical(classes) &&
    !is.complex(classes) && is.null(dim(classes)) &&
    length(classes)==1L && is.finite(classes) &&
    classes==floor(classes) && classes>=2 && classes<=16
  if (!valid_classes) stop("classes must be integer 2..16")
  classes <- as.integer(classes)
  valid_labels <- function(x) {
    is.numeric(x) && !is.logical(x) && !is.complex(x) &&
      is.null(dim(x)) && all(is.finite(x)) &&
      all(x==floor(x)) && all(x>=0) && all(x<classes)
  }
  if (!valid_labels(actual) || !valid_labels(predicted) ||
      length(actual)<1L || length(actual)>512L ||
      length(actual)!=length(predicted)) {
    stop("invalid or unaligned multiclass labels")
  }
  truth <- integer(classes)
  guesses <- integer(classes)
  hits <- integer(classes)
  for (i in seq_along(actual)) {
    a <- as.integer(actual[[i]])+1L
    p <- as.integer(predicted[[i]])+1L
    truth[[a]] <- truth[[a]]+1L
    guesses[[p]] <- guesses[[p]]+1L
    if (a==p) hits[[a]] <- hits[[a]]+1L
  }
  sum_f1 <- 0.0
  for (c in seq_len(classes)) {
    denominator <- truth[[c]]+guesses[[c]]
    if (denominator>0L) sum_f1 <- sum_f1+2*hits[[c]]/denominator
  }
  as.double(sum_f1/classes)
}
