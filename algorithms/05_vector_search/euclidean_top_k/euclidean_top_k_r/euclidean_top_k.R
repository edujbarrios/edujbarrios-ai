# Exact Euclidean Top-K for a query and dense numeric candidate rows.
euclidean_top_k <- function(query,candidates,k) {
  if (!is.numeric(query) || is.complex(query) ||
      length(query)==0L || any(!is.finite(query)) ||
      !is.matrix(candidates) || !is.numeric(candidates) ||
      is.complex(candidates) || nrow(candidates)==0L ||
      ncol(candidates)!=length(query) || any(!is.finite(candidates)) ||
      !is.numeric(k) || is.complex(k) || length(k)!=1L ||
      !is.finite(k) || k!=floor(k) || k<1 || k>nrow(candidates)) {
    stop("invalid vector data, shapes or cutoff")
  }
  distances<-numeric(nrow(candidates))
  for (i in seq_len(nrow(candidates))) {
    distance<-0.0
    for (j in seq_along(query)) {
      delta<-as.double(query[[j]])-as.double(candidates[i,j])
      if (!is.finite(delta)) stop("coordinate difference overflow")
      scale<-max(distance,abs(delta))
      if (scale==0.0) {
        distance<-0.0
      } else {
        distance<-scale*sqrt((distance/scale)^2+(delta/scale)^2)
      }
      if (!is.finite(distance)) stop("distance overflow")
    }
    distances[[i]]<-distance
  }
  as.integer(order(distances,seq_along(distances))[seq_len(k)]-1L)
}
