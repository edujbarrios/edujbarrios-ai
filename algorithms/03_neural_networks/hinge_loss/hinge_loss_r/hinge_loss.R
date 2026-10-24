# Binary mean hinge loss and deterministic score subgradient.
hinge_loss <- function(scores,labels) {
  if (!is.numeric(scores) || is.complex(scores) ||
      !is.numeric(labels) || is.complex(labels) ||
      length(scores)<1L || length(scores)!=length(labels) ||
      any(!is.finite(scores)) || any(!is.finite(labels)) ||
      any(!(labels %in% c(-1,1)))) {
    stop("nonempty finite scores and signed class labels required")
  }
  n<-length(scores)
  total<-0.0
  gradients<-numeric(n)
  for (i in seq_len(n)) {
    margin<-as.double(labels[[i]])*as.double(scores[[i]])
    deficit<-1.0-margin
    if (any(!is.finite(c(margin,deficit)))) stop("margin overflow")
    total<-total+max(0.0,deficit)
    if (!is.finite(total)) stop("loss accumulation overflow")
    gradients[[i]]<-if (margin<1.0) -as.double(labels[[i]])/n else 0.0
  }
  list(loss=as.double(total/n),gradients=gradients)
}
