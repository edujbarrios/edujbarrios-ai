# Add-one smoothed multinomial Naive Bayes for binary count matrices.
multinomial_naive_bayes <- function(counts,labels) {
  valid_counts <- function(v) {
    is.numeric(v) && !is.complex(v) && all(is.finite(v)) &&
      all(v==floor(v)) && all(v>=0) && all(v<=1000000)
  }
  if (!is.matrix(counts) || nrow(counts)==0L || ncol(counts)==0L ||
      !valid_counts(counts) || !is.numeric(labels) || is.complex(labels) ||
      length(labels)!=nrow(counts) || any(!is.finite(labels)) ||
      any(!(labels %in% c(0,1))) || !all(c(0,1) %in% labels)) {
    stop("nonempty count matrix and both binary classes required")
  }
  n<-nrow(counts);d<-ncol(counts)
  tokens<-matrix(0.0,nrow=2L,ncol=d)
  members<-numeric(2);totals<-numeric(2)
  for (i in seq_len(n)) {
    c<-as.integer(labels[[i]])+1L
    members[[c]]<-members[[c]]+1
    for (j in seq_len(d)) {
      value<-as.double(counts[i,j])
      tokens[c,j]<-tokens[c,j]+value
      totals[[c]]<-totals[[c]]+value
      if (tokens[c,j]>9007199254740991 || totals[[c]]>9007199254740991) {
        stop("count exceeds exact double integer limit")
      }
    }
  }
  priors<-log(members/n)
  likelihoods<-matrix(0.0,nrow=2L,ncol=d)
  for (c in 1:2) {
    denominator<-totals[[c]]+d
    likelihoods[c,]<-log((tokens[c,]+1)/denominator)
  }
  if (any(!is.finite(priors)) || any(!is.finite(likelihoods))) {
    stop("invalid model parameters")
  }
  list(log_priors=as.double(priors),log_likelihoods=likelihoods)
}
multinomial_naive_bayes_predict <- function(model,query) {
  if (!is.list(model) || !is.numeric(model$log_priors) ||
      length(model$log_priors)!=2L ||
      !is.matrix(model$log_likelihoods) ||
      !is.numeric(model$log_likelihoods) ||
      nrow(model$log_likelihoods)!=2L ||
      ncol(model$log_likelihoods)!=length(query) ||
      length(query)==0L || !is.numeric(query) || is.complex(query) ||
      any(!is.finite(query)) || any(query!=floor(query)) ||
      any(query<0) || any(query>1000000) ||
      any(!is.finite(model$log_priors)) ||
      any(!is.finite(model$log_likelihoods))) {
    stop("invalid query or fitted model")
  }
  scores<-as.double(model$log_priors)
  for (j in seq_along(query)) {
    for (c in 1:2) {
      term<-as.double(query[[j]])*as.double(model$log_likelihoods[c,j])
      scores[[c]]<-scores[[c]]+term
      if (any(!is.finite(c(term,scores[[c]])))) {
        stop("score overflow")
      }
    }
  }
  as.integer(scores[[2]]>scores[[1]])
}
