# Binary nearest-centroid classifier: fit two means and predict nearest.
nearest_centroid_classifier <- function(rows,labels) {
  if (!is.matrix(rows) || !is.numeric(rows) || is.complex(rows) ||
      nrow(rows)<2L || nrow(rows)>4096L ||
      ncol(rows)<1L || ncol(rows)>64L || any(!is.finite(rows)) ||
      !is.numeric(labels) || is.complex(labels) ||
      length(labels)!=nrow(rows) || any(!is.finite(labels)) ||
      any(!(labels %in% c(0,1))) || !all(c(0,1) %in% labels)) {
    stop("finite labeled n x d matrix with both classes required")
  }
  d<-ncol(rows)
  sums<-matrix(0.0,nrow=2,ncol=d)
  counts<-numeric(2)
  for (i in seq_len(nrow(rows))) {
    c<-as.integer(labels[[i]])+1L
    counts[[c]]<-counts[[c]]+1
    for (j in seq_len(d)) {
      sums[c,j]<-sums[c,j]+as.double(rows[i,j])
      if (!is.finite(sums[c,j])) stop("feature sum overflow")
    }
  }
  for (c in 1:2) for (j in seq_len(d)) {
    sums[c,j]<-sums[c,j]/counts[[c]]
  }
  if (any(!is.finite(sums))) stop("nonfinite centroid")
  sums
}
nearest_centroid_classifier_predict <- function(centroids,query) {
  if (!is.matrix(centroids) || !is.numeric(centroids) ||
      is.complex(centroids) || nrow(centroids)!=2L ||
      !is.numeric(query) || is.complex(query) ||
      length(query)<1L || length(query)>64L ||
      length(query)!=ncol(centroids) || any(!is.finite(centroids)) ||
      any(!is.finite(query))) {
    stop("two finite class centroids and matching query required")
  }
  distances<-numeric(2)
  for (c in 1:2) {
    value<-0.0
    for (j in seq_along(query)) {
      delta<-as.double(query[[j]])-as.double(centroids[c,j])
      if (!is.finite(delta)) stop("coordinate difference overflow")
      scale<-max(value,abs(delta))
      value<-if (scale==0.0) 0.0 else
        scale*sqrt((value/scale)^2+(delta/scale)^2)
      if (!is.finite(value)) stop("distance overflow")
    }
    distances[[c]]<-value
  }
  as.integer(distances[[2]]<distances[[1]])
}
