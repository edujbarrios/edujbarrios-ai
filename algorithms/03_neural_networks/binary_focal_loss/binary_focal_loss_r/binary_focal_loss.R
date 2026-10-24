# Mean binary focal loss, fixed alpha=.25, gamma=2, probability floor 1e-12.
binary_focal_loss <- function(probabilities,labels) {
  if (!is.numeric(probabilities) || is.complex(probabilities) ||
      !is.numeric(labels) || is.complex(labels) ||
      length(probabilities)<1L || length(probabilities)!=length(labels) ||
      any(!is.finite(probabilities)) || any(probabilities<0) ||
      any(probabilities>1) || any(!is.finite(labels)) ||
      any(!(labels %in% c(0,1)))) {
    stop("nonempty finite probabilities and binary labels required")
  }
  total<-0.0
  for (i in seq_along(probabilities)) {
    p<-as.double(probabilities[[i]])
    label<-as.double(labels[[i]])
    pt<-if (label==1) p else 1-p
    q<-max(pt,1e-12)
    alpha<-if (label==1) .25 else .75
    sample<--alpha*(1-q)^2*log(q)
    total<-total+sample
    if (any(!is.finite(c(sample,total)))) stop("focal loss overflow")
  }
  value<-as.double(total/length(probabilities))
  if (!is.finite(value) || value<0) stop("invalid focal loss")
  value
}
