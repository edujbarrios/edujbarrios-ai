# Kruskal MST on nonnegative finite or +Inf symmetric weighted graphs.
kruskal_mst <- function(weights) {
  if (!is.matrix(weights) || !is.numeric(weights) || is.complex(weights) ||
      nrow(weights)<1L || nrow(weights)>64L ||
      nrow(weights)!=ncol(weights) || any(is.na(weights)) ||
      any(weights== -Inf) ||
      any(is.finite(weights) & (weights<0 | weights>1000000)) ||
      any(diag(weights)!=0)) {
    stop("invalid adjacency matrix or edge weights")
  }
  n<-nrow(weights)
  records<-matrix(numeric(),nrow=0,ncol=3L)
  for (i in seq_len(n)) {
    if (i==n) next
    for (j in seq.int(i+1L,n)) {
      a<-as.double(weights[i,j])
      if (a!=weights[j,i]) stop("asymmetric adjacency matrix")
      if (is.finite(a)) records<-rbind(records,c(a,i,j))
    }
  }
  if (nrow(records)>0L) {
    records<-records[order(records[,1],records[,2],records[,3]),,drop=FALSE]
  }
  parent<-seq_len(n)
  sizes<-rep(1L,n)
  root <- function(x) {
    while (parent[[x]]!=x) {
      parent[[x]]<<-parent[[parent[[x]]]]
      x<-parent[[x]]
    }
    x
  }
  chosen<-matrix(integer(),nrow=0,ncol=2L)
  cost<-0.0
  if (nrow(records)>0L) for (e in seq_len(nrow(records))) {
    i<-as.integer(records[e,2])
    j<-as.integer(records[e,3])
    a<-root(i)
    b<-root(j)
    if (a==b) next
    if (sizes[[a]]<sizes[[b]]) {temp<-a;a<-b;b<-temp}
    parent[[b]]<-a
    sizes[[a]]<-sizes[[a]]+sizes[[b]]
    chosen<-rbind(chosen,c(i-1L,j-1L))
    cost<-cost+as.double(records[e,1])
    if (!is.finite(cost)) stop("MST weight overflow")
    if (nrow(chosen)==n-1L) break
  }
  if (nrow(chosen)!=n-1L) stop("graph is disconnected")
  list(edges=matrix(as.integer(chosen),ncol=2L),total=as.double(cost))
}
