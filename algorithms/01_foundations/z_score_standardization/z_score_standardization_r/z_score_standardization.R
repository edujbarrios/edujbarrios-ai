# Column-wise population z-scores from Welford's running moments.
z_score_standardization <- function(data) {
  if (!is.matrix(data) || !is.numeric(data) || is.complex(data) ||
      nrow(data)<1L || ncol(data)<1L || any(!is.finite(data))) {
    stop("expected nonempty finite numeric matrix")
  }
  n<-nrow(data);d<-ncol(data)
  means<-numeric(d);m2<-numeric(d)
  for (i in seq_len(n)) {
    for (j in seq_len(d)) {
      value<-as.double(data[i,j])
      delta<-value-means[[j]]
      if (!is.finite(delta)) stop("mean delta overflow")
      means[[j]]<-means[[j]]+delta/i
      delta2<-value-means[[j]]
      term<-delta*delta2
      if (any(!is.finite(c(means[[j]],delta2,term)))) stop("variance overflow")
      m2[[j]]<-m2[[j]]+term
      if (!is.finite(m2[[j]])) stop("variance sum overflow")
    }
  }
  variances<-m2/n
  if (any(!is.finite(variances)) || any(variances<0)) stop("invalid variance")
  stds<-sqrt(variances)
  normalized<-matrix(0.0,nrow=n,ncol=d)
  for (i in seq_len(n)) {
    for (j in seq_len(d)) {
      if (stds[[j]]==0) next
      delta<-as.double(data[i,j])-means[[j]]
      normalized[i,j]<-delta/stds[[j]]
      if (any(!is.finite(c(delta,normalized[i,j])))) stop("normalization overflow")
    }
  }
  list(means=means,stds=stds,standardized=normalized)
}
