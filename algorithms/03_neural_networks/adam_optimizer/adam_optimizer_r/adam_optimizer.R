# Adam with fixed beta1=.9, beta2=.999 and epsilon=1e-8.
adam_optimizer <- function(parameters,gradients,m,v,learning_rate,step) {
  is_finite_vector <- function(x) is.numeric(x) && !is.complex(x) &&
    all(is.finite(x))
  if (!is_finite_vector(parameters) || length(parameters)<1L ||
      !is_finite_vector(gradients) || length(gradients)!=length(parameters) ||
      !is_finite_vector(m) || length(m)!=length(parameters) ||
      !is_finite_vector(v) || length(v)!=length(parameters) ||
      any(v<0) || !is_finite_vector(learning_rate) ||
      length(learning_rate)!=1L || learning_rate<=0 ||
      !is.numeric(step) || is.complex(step) ||
      length(step)!=1L || !is.finite(step) ||
      step<1 || step!=floor(step)) {
    stop("invalid Adam state, learning rate or step")
  }
  b1<-0.9;b2<-0.999;eps<-1e-8
  c1<--expm1(as.double(step)*log(b1))
  c2<--expm1(as.double(step)*log(b2))
  if (any(!is.finite(c(c1,c2))) || c1<=0 || c2<=0)
    stop("moment correction failed")
  n<-length(parameters)
  next_p<-numeric(n);next_m<-numeric(n);next_v<-numeric(n)
  for (i in seq_len(n)) {
    g<-as.double(gradients[[i]])
    squared<-g*g
    first<-b1*as.double(m[[i]])+(1-b1)*g
    second<-b2*as.double(v[[i]])+(1-b2)*squared
    if (any(!is.finite(c(squared,first,second))) || second<0)
      stop("moment overflow")
    mhat<-first/c1
    vhat<-second/c2
    if (any(!is.finite(c(mhat,vhat))) || vhat<0)
      stop("bias correction overflow")
    delta<-as.double(learning_rate)*(mhat/(sqrt(vhat)+eps))
    theta<-as.double(parameters[[i]])-delta
    if (any(!is.finite(c(delta,theta)))) stop("parameter update overflow")
    next_p[[i]]<-theta;next_m[[i]]<-first;next_v[[i]]<-second
  }
  list(parameters=next_p,m=next_m,v=next_v)
}
