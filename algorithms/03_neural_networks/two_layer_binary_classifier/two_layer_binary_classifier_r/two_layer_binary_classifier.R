# One-hidden-ReLU-unit and sigmoid-output binary classifier training.
stable_sigmoid <- function(z) {
  if (z >= 0) {
    e <- exp(-z)
    1/(1+e)
  } else {
    e <- exp(z)
    e/(1+e)
  }
}

two_layer_predict <- function(params, feature) {
  if (!is.numeric(params) || is.complex(params) ||
      length(params)!=4L || any(!is.finite(params)) ||
      !is.numeric(feature) || is.complex(feature) ||
      length(feature)!=1L || !is.finite(feature)) {
    stop("expected four finite parameters and one finite feature")
  }
  pre <- as.double(params[[1]])*as.double(feature)+as.double(params[[2]])
  if (!is.finite(pre)) stop("preactivation overflow")
  logit <- as.double(params[[3]])*max(0.0,pre)+as.double(params[[4]])
  if (!is.finite(logit)) stop("logit overflow")
  as.double(stable_sigmoid(logit))
}

two_layer_binary_classifier <- function(features, labels, learning_rate, epochs) {
  if (!is.numeric(features) || is.complex(features) ||
      !is.numeric(labels) || is.complex(labels) ||
      length(features)==0L || length(features)!=length(labels) ||
      any(!is.finite(features)) || any(!is.finite(labels)) ||
      any(!(labels %in% c(0,1)))) {
    stop("nonempty finite features and binary labels required")
  }
  if (!is.numeric(learning_rate) || is.complex(learning_rate) ||
      length(learning_rate)!=1L || !is.finite(learning_rate) ||
      learning_rate<=0 || !is.numeric(epochs) || is.complex(epochs) ||
      length(epochs)!=1L || !is.finite(epochs) ||
      epochs!=floor(epochs) || epochs<1) {
    stop("positive finite learning rate and integer epochs required")
  }
  params <- c(w1=1.0,b1=0.5,w2=1.0,b2=0.0)
  for (epoch in seq_len(epochs)) {
    gradient <- numeric(4)
    for (i in seq_along(features)) {
      x <- as.double(features[[i]])
      y <- as.double(labels[[i]])
      pre <- params[[1]]*x+params[[2]]
      if (!is.finite(pre)) stop("preactivation overflow")
      hidden <- max(0.0,pre)
      logit <- params[[3]]*hidden+params[[4]]
      if (!is.finite(logit)) stop("logit overflow")
      error <- stable_sigmoid(logit)-y
      delta <- if (pre>0.0) error*params[[3]] else 0.0
      terms <- c(delta*x,delta,error*hidden,error)
      if (any(!is.finite(terms))) stop("gradient product overflow")
      gradient <- gradient+terms
      if (any(!is.finite(gradient))) stop("gradient sum overflow")
    }
    factor <- learning_rate/length(features)
    updates <- factor*gradient
    if (any(!is.finite(updates))) stop("parameter update overflow")
    params <- params-updates
    if (any(!is.finite(params))) stop("training diverged")
  }
  params
}
