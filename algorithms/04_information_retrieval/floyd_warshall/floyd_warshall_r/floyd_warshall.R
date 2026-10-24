# Floyd-Warshall all-pairs shortest paths on dense directed weighted graphs.
floyd_warshall <- function(adjacency) {
  if (!is.matrix(adjacency) || !is.numeric(adjacency) ||
      is.complex(adjacency) || nrow(adjacency)<1L ||
      nrow(adjacency)>64L || ncol(adjacency)!=nrow(adjacency) ||
      any(is.na(adjacency)) || any(adjacency==-Inf) ||
      any(is.finite(adjacency) &
          (adjacency< -1000000 | adjacency>1000000)) ||
      any(diag(adjacency)!=0)) {
    stop("invalid directed weighted adjacency matrix")
  }
  n<-nrow(adjacency)
  d<-matrix(as.double(adjacency),nrow=n,ncol=n)
  for (k in seq_len(n)) {
    for (i in seq_len(n)) {
      if (is.infinite(d[i,k])) next
      for (j in seq_len(n)) {
        if (is.infinite(d[k,j])) next
        via<-d[i,k]+d[k,j]
        if (!is.finite(via)) stop("shortest path overflow")
        if (via<d[i,j]) d[i,j]<-via
      }
    }
  }
  if (any(diag(d)<0)) stop("negative-weight cycle")
  d
}
