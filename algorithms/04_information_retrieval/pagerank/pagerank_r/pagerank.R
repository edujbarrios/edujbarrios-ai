# Dense PageRank with fixed damping, uniform teleport and dangling rows.
pagerank <- function(weights) {
  if (!is.matrix(weights) || !is.numeric(weights) || is.complex(weights) ||
      nrow(weights)<1L || nrow(weights)>64L ||
      nrow(weights)!=ncol(weights) || any(!is.finite(weights)) ||
      any(weights<0) || any(weights>1000000)) {
    stop("invalid finite nonnegative square weighted adjacency")
  }
  n<-nrow(weights)
  outgoing<-numeric(n)
  for (i in seq_len(n)) {
    outgoing[[i]]<-sum(as.double(weights[i,]))
    if (!is.finite(outgoing[[i]])) stop("row sum overflow")
  }
  rank<-rep(1/n,n)
  for (step in seq_len(100L)) {
    updated<-rep(0.15/n,n)
    for (i in seq_len(n)) {
      if (outgoing[[i]]==0) {
        share<-0.85*rank[[i]]/n
        updated<-updated+share
      } else {
        factor<-0.85*rank[[i]]/outgoing[[i]]
        for (j in seq_len(n)) {
          updated[[j]]<-updated[[j]]+factor*as.double(weights[i,j])
        }
      }
    }
    if (any(!is.finite(updated)) || any(updated<0)) {
      stop("PageRank update overflow")
    }
    rank<-updated
  }
  as.double(rank)
}
