# Unweighted k-nearest-neighbor regression with Euclidean distance.
knn_regression <- function(training,targets,query,k) {
  if (!is.matrix(training) || !is.numeric(training) ||
      is.complex(training) || nrow(training)<1L || nrow(training)>4096L ||
      ncol(training)<1L || ncol(training)>64L ||
      any(!is.finite(training)) ||
      !is.numeric(targets) || is.complex(targets) ||
      length(targets)!=nrow(training) || any(!is.finite(targets)) ||
      !is.numeric(query) || is.complex(query) ||
      length(query)!=ncol(training) || any(!is.finite(query)) ||
      !is.numeric(k) || is.complex(k) || length(k)!=1L ||
      !is.finite(k) || k!=floor(k) || k<1 || k>nrow(training)) {
    stop("invalid numeric dataset, targets, query or k")
  }
  n<-nrow(training)
  d<-ncol(training)
  distances<-numeric(n)
  for (i in seq_len(n)) {
    distance<-0.0
    for (j in seq_len(d)) {
      delta<-as.double(training[i,j])-as.double(query[[j]])
      if (!is.finite(delta)) stop("coordinate difference overflow")
      scale<-max(distance,abs(delta))
      distance<-if (scale==0.0) 0.0 else
        scale*sqrt((distance/scale)^2+(delta/scale)^2)
      if (!is.finite(distance)) stop("Euclidean distance overflow")
    }
    distances[[i]]<-distance
  }
  indices<-order(distances,seq_along(distances))[seq_len(k)]
  prediction<-0.0
  for (index in indices) {
    contribution<-as.double(targets[[index]])/as.double(k)
    if (!is.finite(contribution)) stop("target contribution overflow")
    prediction<-prediction+contribution
    if (!is.finite(prediction)) stop("prediction accumulation overflow")
  }
  as.double(prediction)
}
