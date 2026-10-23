# Deterministic sequential mini-batch gradient descent for y=weight*x+bias.
mini_batch_gradient_descent <- function(x,y,learning_rate,epochs,batch_size) {
  if (!is.numeric(x) || is.complex(x) || !is.numeric(y) ||
      is.complex(y) || length(x)==0L || length(x)!=length(y) ||
      any(!is.finite(x)) || any(!is.finite(y))) {
    stop("nonempty paired finite numeric vectors required")
  }
  if (!is.numeric(learning_rate) || is.complex(learning_rate) ||
      length(learning_rate)!=1L || !is.finite(learning_rate) ||
      learning_rate<=0 || !is.numeric(epochs) || is.complex(epochs) ||
      length(epochs)!=1L || !is.finite(epochs) ||
      epochs!=floor(epochs) || epochs<1 ||
      !is.numeric(batch_size) || is.complex(batch_size) ||
      length(batch_size)!=1L || !is.finite(batch_size) ||
      batch_size!=floor(batch_size) || batch_size<1 ||
      batch_size>length(x)) {
    stop("invalid learning parameters or batch size")
  }
  weight<-0.0; bias<-0.0
  for (epoch in seq_len(epochs)) {
    start<-1L
    while (start<=length(x)) {
      finish<-min(length(x),start+batch_size-1L)
      gw<-0.0;gb<-0.0
      for (i in start:finish) {
        prediction<-weight*as.double(x[[i]])+bias
        error<-prediction-as.double(y[[i]])
        component<-error*as.double(x[[i]])
        if (any(!is.finite(c(prediction,error,component)))) stop("gradient overflow")
        gw<-gw+component;gb<-gb+error
        if (any(!is.finite(c(gw,gb)))) stop("gradient sum overflow")
      }
      factor<-learning_rate/(finish-start+1)
      dw<-factor*gw;db<-factor*gb
      if (any(!is.finite(c(factor,dw,db)))) stop("step overflow")
      weight<-weight-dw;bias<-bias-db
      if (any(!is.finite(c(weight,bias)))) stop("training diverged")
      start<-finish+1L
    }
  }
  list(weight=weight,bias=bias)
}
