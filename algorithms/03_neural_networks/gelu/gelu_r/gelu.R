# Tanh approximation to GELU and its analytical derivative.
gelu <- function(inputs) {
  if (!is.numeric(inputs) || is.complex(inputs) ||
      length(inputs)==0L || any(!is.finite(inputs))) {
    stop("nonempty finite numeric vector required")
  }
  alpha<-0.7978845608028654
  beta<-0.044715
  values<-numeric(length(inputs))
  derivatives<-numeric(length(inputs))
  for (i in seq_along(inputs)) {
    x<-as.double(inputs[[i]])
    square<-x*x
    cubic<-square*x
    u<-alpha*(x+beta*cubic)
    if (any(!is.finite(c(square,cubic,u)))) stop("GELU polynomial overflow")
    t<-tanh(u)
    gate<-0.5*(1.0+t)
    factor<-alpha*(1.0+3.0*beta*square)
    slope<-0.5*x*(1.0-t*t)*factor
    values[[i]]<-x*gate
    derivatives[[i]]<-gate+slope
    if (any(!is.finite(c(factor,slope,values[[i]],derivatives[[i]])))) {
      stop("GELU output overflow")
    }
  }
  list(values=values,derivatives=derivatives)
}
