# Mean Euclidean silhouette for a nontrivial partition of finite observations.
silhouette_score <- function(points,labels) {
  if (!is.matrix(points) || !is.numeric(points) || is.complex(points) ||
      nrow(points)<2L || nrow(points)>128L ||
      ncol(points)<1L || ncol(points)>32L || any(!is.finite(points)) ||
      !is.numeric(labels) || is.complex(labels) ||
      length(labels)!=nrow(points) || any(!is.finite(labels)) ||
      any(labels!=floor(labels)) || any(labels<0) ||
      any(labels>=nrow(points))) {
    stop("invalid finite numeric data or integer cluster labels")
  }
  n<-nrow(points);d<-ncol(points)
  counts<-tabulate(as.integer(labels)+1L,nbins=n)
  clusters<-sum(counts>0)
  if (clusters<2L || clusters>=n) stop("require 2..n-1 populated clusters")
  total<-0.0
  for (i in seq_len(n)) {
    label<-as.integer(labels[[i]])+1L
    if (counts[[label]]==1L) next
    sums<-numeric(n)
    for (j in seq_len(n)) {
      if (i==j) next
      distance<-0.0
      for (col in seq_len(d)) {
        delta<-as.double(points[i,col])-as.double(points[j,col])
        if (!is.finite(delta)) stop("coordinate difference overflow")
        scale<-max(distance,abs(delta))
        distance<-if (scale==0.0) 0.0 else
          scale*sqrt((distance/scale)^2+(delta/scale)^2)
        if (!is.finite(distance)) stop("distance overflow")
      }
      category<-as.integer(labels[[j]])+1L
      sums[[category]]<-sums[[category]]+distance
      if (!is.finite(sums[[category]])) stop("distance sum overflow")
    }
    a<-sums[[label]]/(counts[[label]]-1L)
    others<-which(counts>0 & seq_len(n)!=label)
    b<-min(sums[others]/counts[others])
    if (!is.finite(a) || !is.finite(b)) stop("invalid cluster distance")
    denom<-max(a,b)
    total<-total+if (denom==0.0) 0.0 else (b-a)/denom
    if (!is.finite(total)) stop("silhouette sum overflow")
  }
  score<-as.double(total/n)
  if (!is.finite(score) || score< -1-1e-12 || score>1+1e-12) {
    stop("invalid silhouette coefficient")
  }
  max(-1.0,min(1.0,score))
}
