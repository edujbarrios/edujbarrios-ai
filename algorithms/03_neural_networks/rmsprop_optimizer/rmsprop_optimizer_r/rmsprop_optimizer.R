# RMSProp optimizer: one parameter and squared-gradient-average update.
rmsprop_optimizer <- function(parameters,gradients,previous_average,learning_rate) {
  finite <- function(v) is.numeric(v) && !is.complex(v) && all(is.finite(v))
  n<-length(parameters)
  if (n==0L || !finite(parameters) || !finite(gradients) ||
      !finite(previous_average) || length(gradients)!=n ||
      length(previous_average)!=n || any(previous_average<0) ||
      !finite(learning_rate) || length(learning_rate)!=1L ||
      learning_rate<=0) {
    stop("invalid learning rate, dimensions or squared-gradient state")
  }
  next_params<-numeric(n)
  next_average<-numeric(n)
  for (i in seq_len(n)) {
    g<-as.double(gradients[[i]])
    square<-g*g
    average<-0.9*as.double(previous_average[[i]])+0.1*square
    if (!is.finite(square) || !is.finite(average) || average<0) {
      stop("squared-gradient overflow")
    }
    denominator<-sqrt(average)+1e-8
    step<-as.double(learning_rate)*g/denominator
    updated<-as.double(parameters[[i]])-step
    if (any(!is.finite(c(denominator,step,updated)))) {
      stop("RMSProp update overflow")
    }
    next_params[[i]]<-updated
    next_average[[i]]<-average
  }
  list(parameters=next_params,squared_average=next_average)
}
