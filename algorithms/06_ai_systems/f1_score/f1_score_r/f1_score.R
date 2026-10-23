# Binary F1 score; defined as zero when there are no positives.
f1_score <- function(actual,predicted) {
  valid <- function(x) {
    is.numeric(x) && !is.complex(x) && all(is.finite(x)) &&
      all(x %in% c(0,1))
  }
  if (!valid(actual) || !valid(predicted) ||
      length(actual)==0L || length(actual)!=length(predicted)) {
    stop("nonempty aligned binary labels required")
  }
  tp<-0.0;fp<-0.0;fn<-0.0
  for (i in seq_along(actual)) {
    y<-actual[[i]];p<-predicted[[i]]
    if (y==1 && p==1) tp<-tp+1
    else if (y==0 && p==1) fp<-fp+1
    else if (y==1 && p==0) fn<-fn+1
  }
  denominator<-2*tp+fp+fn
  if (denominator==0) return(0.0)
  as.double(2*tp/denominator)
}
