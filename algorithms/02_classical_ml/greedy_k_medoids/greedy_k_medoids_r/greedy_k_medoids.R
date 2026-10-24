# BUILD-style greedy k-medoid selection on a Euclidean point matrix.
greedy_k_medoids <- function(points,k) {
  if (!is.matrix(points) || !is.numeric(points) || is.complex(points) ||
      nrow(points)<1L || nrow(points)>128L ||
      ncol(points)<1L || ncol(points)>32L || any(!is.finite(points)) ||
      !is.numeric(k) || is.complex(k) || length(k)!=1L ||
      !is.finite(k) || k!=floor(k) || k<1 || k>nrow(points)) {
    stop("invalid numeric point matrix or k")
  }
  n<-nrow(points);d<-ncol(points)
  distances<-matrix(0.0,nrow=n,ncol=n)
  for (i in seq_len(n)) {
    for (j in seq.int(i,n)) {
      metric<-0.0
      for (h in seq_len(d)) {
        delta<-as.double(points[i,h])-as.double(points[j,h])
        if (!is.finite(delta)) stop("coordinate difference overflow")
        scale<-max(metric,abs(delta))
        metric<-if (scale==0.0) 0.0 else
          scale*sqrt((metric/scale)^2+(delta/scale)^2)
        if (!is.finite(metric)) stop("distance overflow")
      }
      distances[i,j]<-metric
      distances[j,i]<-metric
    }
  }
  chosen<-rep(FALSE,n)
  nearest<-rep(Inf,n)
  selected<-integer(k)
  for (step in seq_len(k)) {
    best<-NA_integer_
    best_cost<-Inf
    for (candidate in which(!chosen)) {
      cost<-0.0
      for (i in seq_len(n)) {
        cost<-cost+min(nearest[[i]],distances[i,candidate])
        if (!is.finite(cost)) stop("objective overflow")
      }
      if (is.na(best) || cost<best_cost) {
        best<-as.integer(candidate)
        best_cost<-cost
      }
    }
    selected[[step]]<-as.integer(best-1L)
    chosen[[best]]<-TRUE
    for (i in seq_len(n)) {
      nearest[[i]]<-min(nearest[[i]],distances[i,best])
    }
  }
  selected
}
