# Fixed-length Markov token beam search with deterministic sequence ties.
markov_beam_search <- function(initial,transition,length,width) {
  valid_row <- function(p) {
    is.numeric(p) && !is.complex(p) && length(p)>=1L &&
      all(is.finite(p)) && all(p>0) && all(p<=1) &&
      abs(sum(p)-1)<=1e-9
  }
  n<-base::length(initial)
  if (n<1L || n>8L || !valid_row(initial) ||
      !is.matrix(transition) || !is.numeric(transition) ||
      is.complex(transition) || nrow(transition)!=n ||
      ncol(transition)!=n ||
      !is.numeric(length) || is.complex(length) ||
      base::length(length)!=1L || !is.finite(length) ||
      length!=floor(length) || length<1 || length>8 ||
      !is.numeric(width) || is.complex(width) ||
      base::length(width)!=1L || !is.finite(width) ||
      width!=floor(width) || width<1 || width>8) {
    stop("invalid vocabulary, transition probabilities or beam settings")
  }
  for (i in seq_len(n)) if (!valid_row(transition[i,])) {
    stop("transition row must be a positive normalized distribution")
  }
  length<-as.integer(length);width<-as.integer(width)
  beam<-list(list(score=0.0,path=integer()))
  for (step in seq_len(length)) {
    candidates<-list()
    for (candidate in beam) {
      for (token in seq.int(0L,n-1L)) {
        probability<-if (step==1L) initial[[token+1L]] else
          transition[tail(candidate$path,1L)+1L,token+1L]
        score<-candidate$score+log(probability)
        if (!is.finite(score)) stop("beam score overflow")
        candidates[[base::length(candidates)+1L]]<-
          list(score=score,path=c(candidate$path,as.integer(token)))
      }
    }
    scores<-vapply(candidates,function(x)x$score,numeric(1))
    paths<-do.call(rbind,lapply(candidates,function(x)x$path))
    keys<-as.data.frame(paths)
    order_args<-c(list(-scores),lapply(keys,identity))
    sorted<-do.call(order,order_args)
    beam<-candidates[head(sorted,width)]
  }
  list(tokens=as.integer(beam[[1L]]$path),
       log_probability=as.double(beam[[1L]]$score))
}
