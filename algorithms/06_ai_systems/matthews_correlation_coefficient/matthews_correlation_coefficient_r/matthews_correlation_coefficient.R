# Binary Matthews correlation coefficient for aligned hard labels.
matthews_correlation_coefficient <- function(truth,predictions) {
  if (!is.numeric(truth) || is.complex(truth) ||
      !is.numeric(predictions) || is.complex(predictions) ||
      length(truth)==0L || length(truth)!=length(predictions) ||
      any(!is.finite(truth)) || any(!is.finite(predictions)) ||
      any(!(truth %in% c(0,1))) || any(!(predictions %in% c(0,1)))) {
    stop("nonempty aligned numeric binary labels required")
  }
  n<-length(truth)
  tp<-sum(truth==1 & predictions==1)/n
  tn<-sum(truth==0 & predictions==0)/n
  fp<-sum(truth==0 & predictions==1)/n
  fn<-sum(truth==1 & predictions==0)/n
  denominator<-(tp+fp)*(tp+fn)*(tn+fp)*(tn+fn)
  score<-if (denominator==0.0) 0.0 else
    (tp*tn-fp*fn)/sqrt(denominator)
  if (!is.finite(score) || score< -1-1e-12 || score>1+1e-12) {
    stop("invalid MCC result")
  }
  as.double(max(-1.0,min(1.0,score)))
}
