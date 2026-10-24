# AdamW: one bias-corrected update with decoupled weight decay.
adamw_optimizer <- function(theta,gradient,m,v,step,learning_rate) {
  finite <- function(x) is.numeric(x) && !is.complex(x) && all(is.finite(x))
  n<-length(theta)
  if (n==0L || !finite(theta) || !finite(gradient) || !finite(m) ||
      !finite(v) || length(gradient)!=n || length(m)!=n ||
      length(v)!=n || any(v<0) ||
      !finite(step) || length(step)!=1L || step!=floor(step) ||
      step<1 || step>1000000 || !finite(learning_rate) ||
      length(learning_rate)!=1L || learning_rate<=0) {
    stop("invalid AdamW state, step, dimensions or learning rate")
  }
  c1<-1-0.9^step
  c2<-1-0.999^step
  out<-numeric(n);first<-numeric(n);second<-numeric(n)
  for (i in seq_len(n)) {
    g<-as.double(gradient[[i]])
    square<-g*g
    first[[i]]<-0.9*as.double(m[[i]])+0.1*g
    second[[i]]<-0.999*as.double(v[[i]])+0.001*square
    if (any(!is.finite(c(square,first[[i]],second[[i]]))) ||
        second[[i]]<0) stop("moment update overflow")
    mh<-first[[i]]/c1
    vh<-second[[i]]/c2
    denominator<-sqrt(vh)+1e-8
    adaptive<-mh/denominator
    decay<-0.01*as.double(theta[[i]])
    change<-as.double(learning_rate)*(adaptive+decay)
    out[[i]]<-as.double(theta[[i]])-change
    if (any(!is.finite(c(mh,vh,denominator,adaptive,decay,change,out[[i]])))) {
      stop("AdamW parameter update overflow")
    }
  }
  list(parameters=out,m=first,v=second)
}
