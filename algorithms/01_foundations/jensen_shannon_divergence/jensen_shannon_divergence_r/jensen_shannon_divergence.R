# Base-2 Jensen–Shannon divergence for aligned probability vectors.
jensen_shannon_divergence <- function(p,q) {
  valid <- function(v) {
    is.numeric(v) && !is.complex(v) && length(v)>=1L &&
      length(v)<=4096L && all(is.finite(v)) &&
      all(v>=0) && all(v<=1) && abs(sum(v)-1)<=1e-9
  }
  if (!valid(p) || !valid(q) || length(p)!=length(q)) {
    stop("invalid normalized probability vectors or mismatched lengths")
  }
  js<-0.0
  for (i in seq_along(p)) {
    a<-as.double(p[[i]])
    b<-as.double(q[[i]])
    mix<-0.5*a+0.5*b
    if (a>0) js<-js+0.5*a*log2(a/mix)
    if (b>0) js<-js+0.5*b*log2(b/mix)
    if (!is.finite(js)) stop("JSD arithmetic overflow")
  }
  if (js< -1e-12 || js>1+1e-12) stop("invalid divergence")
  as.double(max(0.0,min(1.0,js)))
}
