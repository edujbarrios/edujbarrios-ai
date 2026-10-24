# Binary ROC AUC from average ranks of exactly tied finite scores.
roc_auc <- function(scores,labels) {
  if (!is.numeric(scores) || is.complex(scores) ||
      !is.numeric(labels) || is.complex(labels) ||
      length(scores)!=length(labels) || length(scores)<2L ||
      any(!is.finite(scores)) || any(!is.finite(labels)) ||
      any(!(labels %in% c(0,1)))) {
    stop("finite score vector and aligned binary labels required")
  }
  positives<-sum(labels)
  n<-length(labels)
  if (positives==0 || positives==n) stop("both classes required")
  indices<-order(as.double(scores),seq_along(scores))
  ordered_scores<-as.double(scores[indices])
  ordered_labels<-as.double(labels[indices])
  rank_sum<-0.0
  start<-1L
  while (start<=n) {
    end<-start
    positive_count<-0.0
    while (end<=n && ordered_scores[[end]]==ordered_scores[[start]]) {
      positive_count<-positive_count+ordered_labels[[end]]
      end<-end+1L
    }
    average_rank<-(start+end-1)/2
    rank_sum<-rank_sum+positive_count*average_rank
    start<-end
  }
  u<-rank_sum-positives*(positives+1)/2
  result<-as.double(u/(positives*(n-positives)))
  if (!is.finite(result) || result< -1e-12 || result>1+1e-12) {
    stop("nonfinite ROC AUC")
  }
  max(0.0,min(1.0,result))
}
