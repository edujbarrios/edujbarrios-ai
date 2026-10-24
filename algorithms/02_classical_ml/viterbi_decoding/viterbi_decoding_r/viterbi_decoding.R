# Two-state HMM Viterbi decoding in log probability space.
viterbi_decoding <- function(observations,initial,transition,emission) {
  normalized <- function(p) {
    is.numeric(p) && !is.complex(p) && length(p)==2L &&
      all(is.finite(p)) && all(p>0) && all(p<=1) &&
      abs(sum(p)-1)<=1e-9
  }
  if (!is.numeric(observations) || is.complex(observations) ||
      length(observations)<1L || length(observations)>4096L ||
      any(!is.finite(observations)) ||
      any(!(observations %in% c(0,1))) ||
      !normalized(initial) ||
      !is.matrix(transition) || !is.numeric(transition) ||
      is.complex(transition) || !identical(dim(transition),c(2L,2L)) ||
      !is.matrix(emission) || !is.numeric(emission) ||
      is.complex(emission) || !identical(dim(emission),c(2L,2L)) ||
      !normalized(transition[1,]) || !normalized(transition[2,]) ||
      !normalized(emission[1,]) || !normalized(emission[2,])) {
    stop("invalid observations or normalized HMM probabilities")
  }
  n<-length(observations)
  back<-matrix(0L,nrow=n,ncol=2L)
  previous<-c(log(initial[[1]])+log(emission[1,observations[[1]]+1L]),
              log(initial[[2]])+log(emission[2,observations[[1]]+1L]))
  if (any(!is.finite(previous))) stop("nonfinite initial log score")
  if (n>1L) for (t in seq.int(2L,n)) {
    current<-numeric(2)
    for (s in 1:2) {
      candidate0<-previous[[1]]+log(transition[1,s])
      candidate1<-previous[[2]]+log(transition[2,s])
      if (any(!is.finite(c(candidate0,candidate1)))) stop("transition log overflow")
      predecessor<-if (candidate1>candidate0) 2L else 1L
      back[t,s]<-as.integer(predecessor-1L)
      current[[s]]<-c(candidate0,candidate1)[[predecessor]]+
        log(emission[s,observations[[t]]+1L])
      if (!is.finite(current[[s]])) stop("emission log overflow")
    }
    previous<-current
  }
  state<-if (previous[[2]]>previous[[1]]) 2L else 1L
  score<-as.double(previous[[state]])
  states<-integer(n)
  for (t in seq.int(n,1L,by=-1L)) {
    states[[t]]<-as.integer(state-1L)
    if (t>1L) state<-back[t,state]+1L
  }
  list(states=states,log_probability=score)
}
