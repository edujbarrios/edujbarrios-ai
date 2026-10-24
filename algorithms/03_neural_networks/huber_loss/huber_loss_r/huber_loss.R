# Batch-mean Huber regression loss and gradients wrt predictions.
huber_loss <- function(predictions,targets,delta) {
  finite <- function(x) is.numeric(x) && !is.complex(x) && all(is.finite(x))
  n<-length(predictions)
  if (n<1L || !finite(predictions) || !finite(targets) ||
      length(targets)!=n || !finite(delta) ||
      length(delta)!=1L || delta<=0) {
    stop("nonempty finite predictions/targets and positive delta required")
  }
  total<-0.0
  gradients<-numeric(n)
  for (i in seq_len(n)) {
    error<-as.double(predictions[[i]])-as.double(targets[[i]])
    if (!is.finite(error)) stop("residual overflow")
    magnitude<-abs(error)
    if (magnitude<=delta) {
      sample<-0.5*error*error
      gradient<-error/n
    } else {
      sample<-as.double(delta)*(magnitude-0.5*as.double(delta))
      gradient<-if (error>0) as.double(delta)/n else -as.double(delta)/n
    }
    if (any(!is.finite(c(sample,gradient)))) stop("Huber arithmetic overflow")
    total<-total+sample
    if (!is.finite(total)) stop("Huber sum overflow")
    gradients[[i]]<-gradient
  }
  result<-as.double(total/n)
  if (!is.finite(result) || result<0) stop("invalid Huber result")
  list(loss=result,gradients=gradients)
}
