# Binary ECE based on predicted-class confidence and equal-width bins.
expected_calibration_error <- function(probabilities,labels,bins) {
  if (!is.numeric(probabilities) || is.complex(probabilities) ||
      !is.numeric(labels) || is.complex(labels) ||
      length(probabilities)==0L || length(probabilities)!=length(labels) ||
      any(!is.finite(probabilities)) || any(probabilities<0) ||
      any(probabilities>1) || any(!is.finite(labels)) ||
      any(!(labels %in% c(0,1))) || !is.numeric(bins) ||
      is.complex(bins) || length(bins)!=1L || !is.finite(bins) ||
      bins!=floor(bins) || bins<1 || bins>100) {
    stop("invalid probabilities, binary labels or bin count")
  }
  bins<-as.integer(bins)
  counts<-integer(bins)
  conf_sums<-numeric(bins)
  correct_sums<-numeric(bins)
  for (i in seq_along(probabilities)) {
    p<-as.double(probabilities[[i]])
    predicted<-as.integer(p>=0.5)
    confidence<-max(p,1-p)
    bin<-min(bins,as.integer(floor(confidence*bins))+1L)
    counts[[bin]]<-counts[[bin]]+1L
    conf_sums[[bin]]<-conf_sums[[bin]]+confidence
    correct_sums[[bin]]<-correct_sums[[bin]]+as.double(predicted==labels[[i]])
    if (any(!is.finite(c(conf_sums[[bin]],correct_sums[[bin]])))) {
      stop("ECE bin overflow")
    }
  }
  result<-0.0
  for (b in seq_len(bins)) {
    if (counts[[b]]==0L) next
    count<-as.double(counts[[b]])
    result<-result+(count/length(probabilities))*
      abs(correct_sums[[b]]/count-conf_sums[[b]]/count)
  }
  if (!is.finite(result) || result<0 || result>1+1e-12) {
    stop("invalid ECE")
  }
  min(1.0,as.double(result))
}
