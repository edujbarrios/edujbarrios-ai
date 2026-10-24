# One-step SGD with gradient-accumulation momentum (beta 0.9).
momentum_sgd <- function(theta,gradient,velocity,learning_rate) {
  finite <- function(x) is.numeric(x) && !is.complex(x) && all(is.finite(x))
  n<-length(theta)
  if (n==0L || !finite(theta) || !finite(gradient) || !finite(velocity) ||
      length(gradient)!=n || length(velocity)!=n ||
      !finite(learning_rate) || length(learning_rate)!=1L ||
      learning_rate<=0) {
    stop("invalid parameters, gradients, velocity or learning rate")
  }
  next_theta<-numeric(n)
  next_velocity<-numeric(n)
  for (i in seq_len(n)) {
    retained<-0.9*as.double(velocity[[i]])
    v<-retained+as.double(gradient[[i]])
    delta<-as.double(learning_rate)*v
    p<-as.double(theta[[i]])-delta
    if (any(!is.finite(c(retained,v,delta,p)))) {
      stop("momentum update overflow")
    }
    next_theta[[i]]<-p
    next_velocity[[i]]<-v
  }
  list(parameters=next_theta,velocity=next_velocity)
}
