# Binary Gaussian naive Bayes with diagonal class variances.
gaussian_naive_bayes <- function(samples, labels, query) {
  if (!is.matrix(samples) || !is.numeric(samples) || is.complex(samples) ||
      nrow(samples)<2L || ncol(samples)<1L ||
      any(!is.finite(samples)) || !is.numeric(labels) ||
      length(labels)!=nrow(samples) || any(!is.finite(labels)) ||
      any(!(labels %in% c(0,1))) || !is.numeric(query) ||
      is.complex(query) || length(query)!=ncol(samples) ||
      any(!is.finite(query))) stop("invalid features, labels or query")
  n <- nrow(samples)
  d <- ncol(samples)
  counts <- integer(2)
  means <- matrix(0.0,nrow=2,ncol=d)
  for(i in seq_len(n)) {
    cls <- as.integer(labels[[i]])+1L
    counts[[cls]] <- counts[[cls]]+1L
    for(j in seq_len(d)) {
      delta <- as.double(samples[i,j])-means[cls,j]
      if(!is.finite(delta)) stop("mean overflow")
      means[cls,j] <- means[cls,j]+delta/counts[[cls]]
      if(!is.finite(means[cls,j])) stop("mean overflow")
    }
  }
  if(any(counts==0L)) stop("both classes are required")
  variances <- matrix(0.0,nrow=2,ncol=d)
  for(i in seq_len(n)) {
    cls <- as.integer(labels[[i]])+1L
    for(j in seq_len(d)) {
      delta <- as.double(samples[i,j])-means[cls,j]
      squared <- delta*delta
      if(!is.finite(delta) || !is.finite(squared)) stop("variance overflow")
      variances[cls,j] <- variances[cls,j]+squared
      if(!is.finite(variances[cls,j])) stop("variance overflow")
    }
  }
  scores <- numeric(2)
  for(cls in seq_len(2)) {
    scores[[cls]] <- log(counts[[cls]]/n)
    for(j in seq_len(d)) {
      v <- max(variances[cls,j]/counts[[cls]],1e-9)
      delta <- as.double(query[[j]])-means[cls,j]
      square <- delta*delta
      term <- log(2*pi*v)+square/v
      if(any(!is.finite(c(v,delta,square,term)))) stop("likelihood overflow")
      scores[[cls]] <- scores[[cls]]-0.5*term
      if(!is.finite(scores[[cls]])) stop("likelihood overflow")
    }
  }
  odds <- scores[[2]]-scores[[1]]
  if(!is.finite(odds)) stop("log odds overflow")
  e <- exp(-abs(odds))
  prob <- if(odds>=0) 1/(1+e) else e/(1+e)
  list(label=as.integer(scores[[2]]>scores[[1]]),probability_1=as.double(prob))
}
