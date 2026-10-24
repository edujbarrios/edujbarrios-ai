# Stable finite log-sum-exp reduction and its softmax gradient.
logsumexp <- function(values) {
  if (!is.numeric(values) || is.complex(values) ||
      length(values)<1L || length(values)>4096L ||
      any(!is.finite(values))) {
    stop("nonempty finite numeric logit vector required")
  }
  maximum<-max(as.double(values))
  weights<-numeric(length(values))
  normalizer<-0.0
  for (i in seq_along(values)) {
    shifted<-as.double(values[[i]])-maximum
    if (!is.finite(shifted)) stop("shifted logit overflow")
    weights[[i]]<-exp(shifted)
    normalizer<-normalizer+weights[[i]]
    if (any(!is.finite(c(weights[[i]],normalizer)))) {
      stop("exponential normalizer overflow")
    }
  }
  if (normalizer<=0) stop("zero normalizer")
  result<-maximum+log(normalizer)
  if (!is.finite(result)) stop("nonfinite logsumexp")
  gradient<-as.double(weights/normalizer)
  if (any(!is.finite(gradient))) stop("nonfinite gradient")
  list(value=as.double(result),gradient=gradient)
}
