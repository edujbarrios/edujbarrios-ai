# Label-smoothed multiclass cross-entropy and logit gradient.
label_smoothed_cross_entropy <- function(logits,target,epsilon) {
  if (!is.numeric(logits) || is.complex(logits) ||
      length(logits)<1L || length(logits)>4096L ||
      any(!is.finite(logits)) ||
      !is.numeric(target) || is.complex(target) ||
      length(target)!=1L || !is.finite(target) ||
      target!=floor(target) || target<0 || target>=length(logits) ||
      !is.numeric(epsilon) || is.complex(epsilon) ||
      length(epsilon)!=1L || !is.finite(epsilon) ||
      epsilon<0 || epsilon>1) {
    stop("invalid logits, target or smoothing")
  }
  n<-length(logits)
  shifted<-as.double(logits)-max(as.double(logits))
  if (any(!is.finite(shifted))) stop("logit difference overflow")
  weights<-exp(shifted)
  total<-sum(weights)
  if (!is.finite(total) || total<=0) stop("invalid softmax normalizer")
  logz<-log(total)
  target_distribution<-rep(epsilon/n,n)
  target_distribution[[as.integer(target)+1L]]<-
    target_distribution[[as.integer(target)+1L]]+1-epsilon
  loss<-0.0
  gradients<-numeric(n)
  for (i in seq_len(n)) {
    term<-target_distribution[[i]]*(logz-shifted[[i]])
    loss<-loss+term
    gradients[[i]]<-weights[[i]]/total-target_distribution[[i]]
    if (any(!is.finite(c(term,loss,gradients[[i]])))) {
      stop("loss or gradient overflow")
    }
  }
  if (loss< -1e-12) stop("negative cross-entropy")
  list(loss=as.double(max(0,loss)),gradients=as.double(gradients))
}
