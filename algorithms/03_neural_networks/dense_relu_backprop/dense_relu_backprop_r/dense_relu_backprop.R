# Backward gradients of an affine dense layer and ReLU activation.
dense_relu_backprop <- function(x, weights, bias, upstream) {
  if(!is.numeric(x) || is.complex(x) || length(x)<1L ||
     any(!is.finite(x)) || !is.matrix(weights) ||
     !is.numeric(weights) || is.complex(weights) ||
     nrow(weights)<1L || ncol(weights)!=length(x) ||
     any(!is.finite(weights)) || !is.numeric(bias) ||
     is.complex(bias) || length(bias)!=nrow(weights) ||
     any(!is.finite(bias)) || !is.numeric(upstream) ||
     is.complex(upstream) || length(upstream)!=nrow(weights) ||
     any(!is.finite(upstream))) {
    stop("invalid dense ReLU backward inputs")
  }
  d<-length(x);o<-nrow(weights)
  dx<-numeric(d)
  dw<-matrix(0.0,nrow=o,ncol=d)
  db<-numeric(o)
  for(j in seq_len(o)) {
    z<-0.0
    for(i in seq_len(d)) {
      product<-as.double(weights[j,i])*as.double(x[[i]])
      if(!is.finite(product)) stop("multiplication overflow")
      z<-z+product
      if(!is.finite(z)) stop("affine sum overflow")
    }
    z<-z+as.double(bias[[j]])
    if(!is.finite(z)) stop("bias addition overflow")
    dz<-if(z>0) as.double(upstream[[j]]) else 0.0
    db[[j]]<-dz
    for(i in seq_len(d)) {
      wg<-dz*as.double(x[[i]])
      term<-as.double(weights[j,i])*dz
      if(any(!is.finite(c(wg,term)))) stop("gradient overflow")
      dw[j,i]<-wg
      dx[[i]]<-dx[[i]]+term
      if(!is.finite(dx[[i]])) stop("gradient sum overflow")
    }
  }
  list(grad_input=dx,grad_weights=dw,grad_bias=db)
}
