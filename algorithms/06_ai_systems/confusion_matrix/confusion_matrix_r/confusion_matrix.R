# Return named counts in (TN,FP,FN,TP) order.
confusion_matrix <- function(actual, predicted) {
  check <- function(v) {
    is.numeric(v) && !is.complex(v) && all(is.finite(v)) &&
      all(v %in% c(0,1))
  }
  if (!check(actual) || !check(predicted) ||
      length(actual)==0L || length(actual)!=length(predicted)) {
    stop("nonempty aligned binary integer labels required")
  }
  counts<-c(TN=0L,FP=0L,FN=0L,TP=0L)
  for (i in seq_along(actual)) {
    index<-as.integer(actual[[i]])*2L+as.integer(predicted[[i]])+1L
    counts[[index]]<-counts[[index]]+1L
  }
  counts
}
