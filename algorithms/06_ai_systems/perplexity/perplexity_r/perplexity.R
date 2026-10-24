# Token-level language-model perplexity from finite natural log probabilities.
perplexity <- function(log_probabilities) {
  if (!is.numeric(log_probabilities) || is.complex(log_probabilities) ||
      length(log_probabilities)<1L || length(log_probabilities)>4096L ||
      any(!is.finite(log_probabilities)) ||
      any(log_probabilities>0)) {
    stop("expected 1..4096 finite nonpositive natural log probabilities")
  }
  n<-length(log_probabilities)
  mean_nll<-0.0
  for (value in log_probabilities) {
    contribution<-(-as.double(value))/n
    mean_nll<-mean_nll+contribution
    if (any(!is.finite(c(contribution,mean_nll)))) {
      stop("mean negative log likelihood overflow")
    }
  }
  result<-exp(mean_nll)
  if (!is.finite(result) || result<1) {
    stop("perplexity exponential overflow")
  }
  as.double(result)
}
