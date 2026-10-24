# Binary Brier score: mean squared error of class-one probabilities.
brier_score <- function(probabilities,labels) {
  if (!is.numeric(probabilities) || is.complex(probabilities) ||
      !is.numeric(labels) || is.complex(labels) ||
      length(probabilities)==0L || length(probabilities)!=length(labels) ||
      any(!is.finite(probabilities)) || any(probabilities<0) ||
      any(probabilities>1) || any(!is.finite(labels)) ||
      any(!(labels %in% c(0,1)))) {
    stop("finite class probabilities in [0,1] and binary labels required")
  }
  total<-0.0
  for (i in seq_along(probabilities)) {
    error<-as.double(probabilities[[i]])-as.double(labels[[i]])
    total<-total+error*error
    if (!is.finite(total)) stop("Brier accumulation overflow")
  }
  result<-as.double(total/length(probabilities))
  if (!is.finite(result) || result<0 || result>1+1e-12) {
    stop("invalid Brier score")
  }
  min(1.0,result)
}
