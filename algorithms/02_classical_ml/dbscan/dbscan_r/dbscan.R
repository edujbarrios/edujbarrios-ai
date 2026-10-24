# Deterministic DBSCAN with self-inclusive Euclidean neighborhoods.
dbscan <- function(points,epsilon,min_samples) {
  if (!is.matrix(points) || !is.numeric(points) || is.complex(points) ||
      nrow(points)<1L || nrow(points)>1024L ||
      ncol(points)<1L || ncol(points)>64L || any(!is.finite(points)) ||
      !is.numeric(epsilon) || is.complex(epsilon) ||
      length(epsilon)!=1L || !is.finite(epsilon) || epsilon<=0 ||
      !is.numeric(min_samples) || is.complex(min_samples) ||
      length(min_samples)!=1L || !is.finite(min_samples) ||
      min_samples!=floor(min_samples) || min_samples<1 ||
      min_samples>nrow(points)) {
    stop("invalid point matrix, epsilon or min_samples")
  }
  n<-nrow(points);d<-ncol(points)
  adjacency<-matrix(FALSE,nrow=n,ncol=n)
  counts<-integer(n)
  for (i in seq_len(n)) {
    for (j in seq.int(i,n)) {
      distance<-0.0
      for (col in seq_len(d)) {
        diff<-as.double(points[i,col])-as.double(points[j,col])
        if (!is.finite(diff)) stop("difference overflow")
        scale<-max(distance,abs(diff))
        if (scale==0) {
          distance<-0.0
        } else {
          distance<-scale*sqrt((distance/scale)^2+(diff/scale)^2)
        }
        if (!is.finite(distance)) stop("distance overflow")
      }
      if (distance<=epsilon) {
        adjacency[i,j]<-TRUE;adjacency[j,i]<-TRUE
        counts[[i]]<-counts[[i]]+1L
        if (i!=j) counts[[j]]<-counts[[j]]+1L
      }
    }
  }
  visited<-rep(FALSE,n)
  labels<-rep(-1L,n)
  cluster<-0L
  for (i in seq_len(n)) {
    if (visited[[i]]) next
    visited[[i]]<-TRUE
    if (counts[[i]]<min_samples) next
    labels[[i]]<-cluster
    queued<-rep(FALSE,n)
    queue<-which(adjacency[i,] & seq_len(n)!=i)
    queued[queue]<-TRUE
    front<-1L
    while (front<=length(queue)) {
      j<-queue[[front]];front<-front+1L
      if (!visited[[j]]) {
        visited[[j]]<-TRUE
        if (counts[[j]]>=min_samples) {
          for (h in which(adjacency[j,] & !visited & !queued)) {
            queue<-c(queue,h);queued[[h]]<-TRUE
          }
        }
      }
      if (labels[[j]]==-1L) labels[[j]]<-cluster
    }
    cluster<-cluster+1L
  }
  as.integer(labels)
}
