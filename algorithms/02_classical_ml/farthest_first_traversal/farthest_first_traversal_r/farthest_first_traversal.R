# Greedy k-center farthest-first selection; 0-based center indices.
farthest_first_traversal <- function(points,k) {
  if (!is.matrix(points) || !is.numeric(points) || is.complex(points) ||
      nrow(points)==0L || ncol(points)==0L ||
      any(!is.finite(points)) ||
      !is.numeric(k) || is.complex(k) || length(k)!=1L ||
      !is.finite(k) || k!=floor(k) || k<1 || k>nrow(points)) {
    stop("nonempty finite point matrix and integer k required")
  }
  n<-nrow(points);d<-ncol(points)
  centers<-integer(k)
  used<-rep(FALSE,n)
  nearest<-rep(Inf,n)
  for (step in seq_len(k)) {
    if (step==1L) {
      next_idx<-1L
    } else {
      choices<-which(!used)
      next_idx<-choices[[which.max(nearest[choices])]]
    }
    centers[[step]]<-as.integer(next_idx-1L)
    used[[next_idx]]<-TRUE
    for (i in which(!used)) {
      distance<-0.0
      for (j in seq_len(d)) {
        delta<-as.double(points[i,j])-as.double(points[next_idx,j])
        if (!is.finite(delta)) stop("coordinate difference overflow")
        scale<-max(distance,abs(delta))
        if (scale==0.0) {
          distance<-0.0
        } else {
          distance<-scale*sqrt((distance/scale)^2+(delta/scale)^2)
        }
        if (!is.finite(distance)) stop("Euclidean distance overflow")
      }
      nearest[[i]]<-min(nearest[[i]],distance)
    }
  }
  centers
}
