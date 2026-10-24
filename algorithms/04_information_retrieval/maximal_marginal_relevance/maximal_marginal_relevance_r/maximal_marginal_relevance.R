# Greedy maximal marginal relevance with directed candidate similarity.
maximal_marginal_relevance <- function(relevance,similarities,lambda,k) {
  valid_range <- function(x,lo,hi) {
    is.numeric(x) && !is.complex(x) && all(is.finite(x)) &&
      all(x>=lo) && all(x<=hi)
  }
  n<-length(relevance)
  if (n<1L || !valid_range(relevance,0,1) ||
      !is.matrix(similarities) || !valid_range(similarities,-1,1) ||
      nrow(similarities)!=n || ncol(similarities)!=n ||
      !valid_range(lambda,0,1) || length(lambda)!=1L ||
      !is.numeric(k) || is.complex(k) || length(k)!=1L ||
      !is.finite(k) || k!=floor(k) || k<1 || k>n) {
    stop("invalid relevance, similarities, lambda or k")
  }
  used<-rep(FALSE,n)
  max_sim<-rep(-Inf,n)
  selected<-integer(k)
  for (step in seq_len(k)) {
    best_idx<-NA_integer_
    best_score<--Inf
    for (i in which(!used)) {
      score<-lambda*relevance[[i]] -
        if (step==1L) 0.0 else (1-lambda)*max_sim[[i]]
      if (!is.finite(score)) stop("MMR score overflow")
      if (is.na(best_idx) || score>best_score) {
        best_idx<-as.integer(i)
        best_score<-score
      }
    }
    selected[[step]]<-best_idx-1L
    used[[best_idx]]<-TRUE
    for (i in which(!used)) {
      max_sim[[i]]<-max(max_sim[[i]],similarities[i,best_idx])
    }
  }
  selected
}
