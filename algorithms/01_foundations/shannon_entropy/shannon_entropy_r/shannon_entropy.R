# Shannon entropy of a finite categorical distribution, measured in bits.
shannon_entropy <- function(probabilities) {
  if (!is.numeric(probabilities) || is.complex(probabilities) ||
      length(probabilities)<1L || length(probabilities)>4096L ||
      any(!is.finite(probabilities)) || any(probabilities<0) ||
      any(probabilities>1)) {
    stop("nonempty finite probability vector in [0,1] required")
  }
  total<-sum(as.double(probabilities))
  if (!is.finite(total) || abs(total-1)>1e-9) {
    stop("probabilities must sum to one")
  }
  entropy<-0.0
  for (p in probabilities) {
    if (p>0) entropy<-entropy-as.double(p)*log2(as.double(p))
    if (!is.finite(entropy)) stop("entropy arithmetic overflow")
  }
  if (entropy< -1e-12) stop("negative entropy")
  as.double(max(0.0,entropy))
}
