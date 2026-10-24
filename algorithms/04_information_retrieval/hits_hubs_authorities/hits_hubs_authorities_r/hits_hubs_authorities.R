# HITS coupled link authority and hub scores with fixed power iterations.
hits_hubs_authorities <- function(weights) {
  if (!is.matrix(weights) || !is.numeric(weights) || is.complex(weights) ||
      nrow(weights)<1L || nrow(weights)>64L ||
      nrow(weights)!=ncol(weights) ||
      any(!is.finite(weights)) || any(weights<0) || any(weights>1)) {
    stop("finite square adjacency matrix with edge weights in [0,1] required")
  }
  n<-nrow(weights)
  hubs<-rep(1/sqrt(n),n)
  authorities<-numeric(n)
  stable_norm <- function(x) {
    norm<-0.0
    for (v in x) {
      scale<-max(norm,abs(v))
      norm<-if (scale==0) 0.0 else scale*sqrt((norm/scale)^2+(v/scale)^2)
    }
    norm
  }
  for (step in seq_len(100L)) {
    raw<-numeric(n)
    for (j in seq_len(n)) {
      for (i in seq_len(n)) raw[[j]]<-raw[[j]]+as.double(weights[i,j])*hubs[[i]]
    }
    norm<-stable_norm(raw)
    if (any(!is.finite(raw)) || !is.finite(norm)) stop("authority score overflow")
    authorities<-if (norm>0) raw/norm else numeric(n)
    raw<-numeric(n)
    for (i in seq_len(n)) {
      for (j in seq_len(n)) raw[[i]]<-raw[[i]]+as.double(weights[i,j])*authorities[[j]]
    }
    norm<-stable_norm(raw)
    if (any(!is.finite(raw)) || !is.finite(norm)) stop("hub score overflow")
    hubs<-if (norm>0) raw/norm else numeric(n)
  }
  list(authorities=as.double(authorities),hubs=as.double(hubs))
}
