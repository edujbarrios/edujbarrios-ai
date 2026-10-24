# Base-2 KL divergence; positive infinity for Q missing P support.
kullback_leibler_divergence <- function(p,q) {
  valid <- function(v) {
    is.numeric(v) && !is.complex(v) && length(v)>=1L &&
      length(v)<=4096L && all(is.finite(v)) &&
      all(v>=0) && all(v<=1) && abs(sum(v)-1)<=1e-9
  }
  if (!valid(p) || !valid(q) || length(p)!=length(q)) {
    stop("invalid aligned normalized categorical distributions")
  }
  if (any(p>0 & q==0)) return(Inf)
  result<-0.0
  for (i in seq_along(p)) {
    a<-as.double(p[[i]]);b<-as.double(q[[i]])
    if (a>0) {
      term<-a*(log2(a)-log2(b))
      if (!is.finite(term)) stop("KL term overflow")
      result<-result+term
      if (!is.finite(result)) stop("KL accumulation overflow")
    }
  }
  if (result< -1e-12) stop("negative relative entropy")
  as.double(max(0.0,result))
}
