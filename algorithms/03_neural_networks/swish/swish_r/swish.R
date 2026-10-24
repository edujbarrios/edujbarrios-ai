# Stable Swish (SiLU) activation with its elementwise derivative.
swish <- function(inputs) {
  if (!is.numeric(inputs) || is.complex(inputs) ||
      length(inputs)==0L || any(!is.finite(inputs))) {
    stop("nonempty finite real input vector required")
  }
  values<-numeric(length(inputs))
  derivatives<-numeric(length(inputs))
  for (i in seq_along(inputs)) {
    x<-as.double(inputs[[i]])
    if (x>=0) {
      z<-exp(-x)
      s<-1.0/(1.0+z)
    } else {
      z<-exp(x)
      s<-z/(1.0+z)
    }
    values[[i]]<-x*s
    derivatives[[i]]<-s+(x*s)*(1.0-s)
    if (!is.finite(values[[i]]) || !is.finite(derivatives[[i]])) {
      stop("nonfinite Swish output")
    }
  }
  list(values=values,derivatives=derivatives)
}
