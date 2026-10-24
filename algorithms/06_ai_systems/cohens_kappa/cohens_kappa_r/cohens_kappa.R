# Binary Cohen kappa corrected for chance agreement from empirical marginals.
cohens_kappa <- function(first,second) {
  valid <- function(x) is.numeric(x) && !is.complex(x) &&
    all(is.finite(x)) && all(x %in% c(0,1))
  if (!valid(first) || !valid(second) ||
      length(first)==0L || length(first)!=length(second)) {
    stop("nonempty aligned binary integer labels required")
  }
  n<-length(first)
  pa<-sum(first==0)/n
  pb<-sum(second==0)/n
  observed<-sum(first==second)/n
  expected<-pa*pb+(1-pa)*(1-pb)
  denominator<-1-expected
  if (!is.finite(denominator) || denominator<=0) {
    stop("kappa undefined with expected agreement one")
  }
  result<-(observed-expected)/denominator
  if (!is.finite(result) || result< -1-1e-12 || result>1+1e-12){
    stop("invalid kappa")
  }
  as.double(max(-1,min(1,result)))
}
