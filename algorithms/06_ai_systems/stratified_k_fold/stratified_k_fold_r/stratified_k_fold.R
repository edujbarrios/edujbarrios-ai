# Stratify binary observations into deterministic balanced folds.
stratified_k_fold <- function(labels,k) {
  if (!is.numeric(labels) || is.complex(labels) ||
      length(labels)<2L || length(labels)>4096L ||
      any(!is.finite(labels)) || any(!(labels %in% c(0,1))) ||
      !is.numeric(k) || is.complex(k) || length(k)!=1L ||
      !is.finite(k) || k!=floor(k) || k<2 || k>length(labels)) {
    stop("invalid binary labels or fold count")
  }
  counts<-c(sum(labels==0),sum(labels==1))
  if (any(counts<k)) stop("each class needs at least k observations")
  folds<-integer(length(labels))
  counter<-0L
  for (class in c(0,1)) {
    for (i in which(labels==class)) {
      folds[[i]]<-as.integer(counter %% k)
      counter<-counter+1L
    }
  }
  folds
}
