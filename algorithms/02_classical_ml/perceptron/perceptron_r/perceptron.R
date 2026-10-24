# Online multifeature binary perceptron with deterministic training order.
perceptron_predict <- function(weights,features,bias) {
  if (!is.numeric(weights) || is.complex(weights) ||
      length(weights)==0L || any(!is.finite(weights)) ||
      !is.numeric(features) || is.complex(features) ||
      length(features)!=length(weights) || any(!is.finite(features)) ||
      !is.numeric(bias) || is.complex(bias) ||
      length(bias)!=1L || !is.finite(bias)) {
    stop("invalid inference dimensions or values")
  }
  score<-0.0
  for (j in seq_along(weights)) {
    term<-as.double(weights[[j]])*as.double(features[[j]])
    if (!is.finite(term)) stop("linear product overflow")
    score<-score+term
    if (!is.finite(score)) stop("linear sum overflow")
  }
  score<-score+bias
  if (!is.finite(score)) stop("linear score overflow")
  as.integer(score>0.0)
}
perceptron <- function(features,labels,learning_rate,epochs) {
  if (!is.matrix(features) || !is.numeric(features) ||
      is.complex(features) || nrow(features)<1L || ncol(features)<1L ||
      any(!is.finite(features)) || !is.numeric(labels) ||
      is.complex(labels) || length(labels)!=nrow(features) ||
      any(!is.finite(labels)) || any(!(labels %in% c(0,1))) ||
      !is.numeric(learning_rate) || is.complex(learning_rate) ||
      length(learning_rate)!=1L || !is.finite(learning_rate) ||
      learning_rate<=0 || !is.numeric(epochs) || is.complex(epochs) ||
      length(epochs)!=1L || !is.finite(epochs) ||
      epochs!=floor(epochs) || epochs<1) {
    stop("invalid training inputs or parameters")
  }
  d<-ncol(features)
  w<-numeric(d)
  bias<-0.0
  for (epoch in seq_len(epochs)) {
    for (i in seq_len(nrow(features))) {
      pred<-perceptron_predict(w,features[i,],bias)
      label<-as.integer(labels[[i]])
      if (pred==label) next
      delta<-if (label==1L) as.double(learning_rate) else -as.double(learning_rate)
      for (j in seq_len(d)) {
        update<-delta*as.double(features[i,j])
        if (!is.finite(update)) stop("weight product overflow")
        w[[j]]<-w[[j]]+update
        if (!is.finite(w[[j]])) stop("weight update overflow")
      }
      bias<-bias+delta
      if (!is.finite(bias)) stop("bias update overflow")
    }
  }
  list(weights=w,bias=bias)
}
