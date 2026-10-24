# Dense directed Dijkstra with zero meaning absent edge.
dijkstra_shortest_path <- function(weights,source) {
  if (!is.matrix(weights) || !is.numeric(weights) || is.complex(weights) ||
      nrow(weights)<1L || nrow(weights)>128L ||
      nrow(weights)!=ncol(weights) || any(!is.finite(weights)) ||
      any(weights<0) || any(weights>1000000) ||
      !is.numeric(source) || is.complex(source) ||
      length(source)!=1L || !is.finite(source) ||
      source!=floor(source) || source<0 || source>=nrow(weights)) {
    stop("invalid weighted directed adjacency matrix or source")
  }
  n<-nrow(weights)
  distance<-rep(Inf,n)
  visited<-rep(FALSE,n)
  distance[[as.integer(source)+1L]]<-0.0
  for (step in seq_len(n)) {
    choices<-which(!visited & is.finite(distance))
    if (length(choices)==0L) break
    u<-choices[[which.min(distance[choices])]]
    visited[[u]]<-TRUE
    for (v in seq_len(n)) {
      w<-as.double(weights[u,v])
      if (w==0 || visited[[v]]) next
      candidate<-distance[[u]]+w
      if (!is.finite(candidate)) stop("distance overflow")
      if (candidate<distance[[v]]) distance[[v]]<-candidate
    }
  }
  distance[!is.finite(distance)]<--1.0
  as.double(distance)
}
