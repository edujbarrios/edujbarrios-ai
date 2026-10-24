# Training-mode featurewise batch normalization on a numeric matrix.
batch_normalization <- function(x,gamma,beta) {
  finite_bounded <- function(v) {
    is.numeric(v) && !is.complex(v) && all(is.finite(v)) &&
      all(abs(v)<=1e6)
  }
  if (!is.matrix(x) || !finite_bounded(x) ||
      nrow(x)<1L || nrow(x)>512L || ncol(x)<1L || ncol(x)>32L ||
      !finite_bounded(gamma) || !finite_bounded(beta) ||
      length(gamma)!=ncol(x) || length(beta)!=ncol(x)) {
    stop("invalid finite batch or per-feature affine parameters")
  }
  n<-nrow(x);d<-ncol(x)
  mu<-numeric(d);variance<-numeric(d);scale<-numeric(d)
  for (j in seq_len(d)) {
    mu[[j]]<-sum(as.double(x[,j])/n)
    variance[[j]]<-sum((as.double(x[,j])-mu[[j]])^2/n)
    scale[[j]]<-as.double(gamma[[j]])/sqrt(variance[[j]]+1e-5)
  }
  if (any(!is.finite(c(mu,variance,scale)))) stop("feature moment overflow")
  output<-matrix(0.0,nrow=n,ncol=d)
  for (i in seq_len(n)) {
    for (j in seq_len(d)) {
      output[i,j]<-(as.double(x[i,j])-mu[[j]])*scale[[j]]+
        as.double(beta[[j]])
    }
  }
  if (any(!is.finite(output))) stop("normalized output overflow")
  output
}
