# Numerically stable temperature-scaled softmax over one logit vector.
temperature_softmax <- function(logits,temperature) {
  if (!is.numeric(logits) || is.complex(logits) ||
      length(logits)<1L || length(logits)>4096L || any(!is.finite(logits)) ||
      !is.numeric(temperature) || is.complex(temperature) ||
      length(temperature)!=1L || !is.finite(temperature) ||
      temperature<=0) {
    stop("nonempty finite logits and positive finite temperature required")
  }
  maximum<-max(as.double(logits))
  weights<-numeric(length(logits))
  total<-0.0
  for (i in seq_along(logits)) {
    shifted<-as.double(logits[[i]])-maximum
    scaled<-shifted/as.double(temperature)
    if (any(!is.finite(c(shifted,scaled)))) stop("softmax scaling overflow")
    weights[[i]]<-exp(scaled)
    total<-total+weights[[i]]
    if (any(!is.finite(c(weights[[i]],total)))) stop("weight overflow")
  }
  if (total<=0) stop("invalid normalizer")
  probabilities<-as.double(weights/total)
  if (any(!is.finite(probabilities))) stop("nonfinite probabilities")
  probabilities
}
