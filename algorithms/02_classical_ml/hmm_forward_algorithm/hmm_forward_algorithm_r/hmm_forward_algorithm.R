# Two-hidden-state Forward recursion in stable natural-log space.
hmm_forward_algorithm <- function(observations,initial,transition,emission) {
  valid_pair <- function(x) {
    is.numeric(x) && !is.complex(x) && length(x)==2L &&
      all(is.finite(x)) && all(x>0) && all(x<=1) &&
      abs(sum(x)-1)<=1e-9
  }
  valid_matrix <- function(m) {
    is.matrix(m) && is.numeric(m) && !is.complex(m) &&
      identical(dim(m),c(2L,2L)) &&
      valid_pair(m[1,]) && valid_pair(m[2,])
  }
  if (!is.numeric(observations) || is.complex(observations) ||
      length(observations)<1L || length(observations)>4096L ||
      any(!is.finite(observations)) ||
      any(!(observations %in% c(0,1))) ||
      !valid_pair(initial) || !valid_matrix(transition) ||
      !valid_matrix(emission)) {
    stop("invalid observations or normalized HMM distributions")
  }
  lse <- function(a,b) {
    high<-max(a,b)
    high+log1p(exp(min(a,b)-high))
  }
  prev<-c(
    log(initial[[1]])+log(emission[1,observations[[1]]+1L]),
    log(initial[[2]])+log(emission[2,observations[[1]]+1L])
  )
  if (any(!is.finite(prev))) stop("initial log-likelihood overflow")
  if (length(observations)>1L) {
    for (t in seq.int(2L,length(observations))) {
      nxt<-numeric(2)
      for (s in 1:2) {
        a<-prev[[1]]+log(transition[1,s])
        b<-prev[[2]]+log(transition[2,s])
        if (any(!is.finite(c(a,b)))) stop("transition score overflow")
        nxt[[s]]<-lse(a,b)+log(emission[s,observations[[t]]+1L])
        if (!is.finite(nxt[[s]])) stop("emission score overflow")
      }
      prev<-nxt
    }
  }
  result<-lse(prev[[1]],prev[[2]])
  if (!is.finite(result) || result>1e-12) {
    stop("invalid observation log likelihood")
  }
  as.double(min(0,result))
}
