# Gaussian RBF Gram matrix on bounded dense Euclidean data.
gaussian_rbf_kernel <- function(points,sigma) {
  if (!is.matrix(points) || !is.numeric(points) || is.complex(points) ||
      nrow(points)<1L || nrow(points)>128L ||
      ncol(points)<1L || ncol(points)>32L ||
      any(!is.finite(points)) || any(abs(points)>1e6) ||
      !is.numeric(sigma) || is.complex(sigma) ||
      length(sigma)!=1L || !is.finite(sigma) ||
      sigma<1e-6 || sigma>1e6) {
    stop("invalid finite feature matrix or sigma")
  }
  n<-nrow(points);d<-ncol(points)
  kernel<-matrix(0.0,nrow=n,ncol=n)
  for (i in seq_len(n)) {
    for (j in seq.int(i,n)) {
      distance<-0.0
      for (c in seq_len(d)) {
        delta<-as.double(points[i,c])-as.double(points[j,c])
        scale<-max(distance,abs(delta))
        distance<-if (scale==0.0) 0.0 else
          scale*sqrt((distance/scale)^2+(delta/scale)^2)
      }
      scaled<-distance/sigma
      value<-exp(-0.5*scaled*scaled)
      if (!is.finite(value) || value<0 || value>1) {
        stop("Gaussian kernel arithmetic overflow")
      }
      kernel[i,j]<-value
      kernel[j,i]<-value
    }
  }
  kernel
}
