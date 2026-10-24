# Deterministic nucleus prefix filtering of normalized token probabilities.
top_p_nucleus_filtering <- function(probabilities,threshold) {
  if (!is.numeric(probabilities) || is.complex(probabilities) ||
      length(probabilities)<1L || length(probabilities)>4096L ||
      any(!is.finite(probabilities)) || any(probabilities<0) ||
      any(probabilities>1) || abs(sum(probabilities)-1)>1e-9 ||
      !is.numeric(threshold) || is.complex(threshold) ||
      length(threshold)!=1L || !is.finite(threshold) ||
      threshold<=0 || threshold>1) {
    stop("invalid normalized distribution or top-p threshold")
  }
  ranked<-order(-as.double(probabilities),seq_along(probabilities))
  selected<-integer()
  cumulative<-0.0
  for (i in ranked) {
    cumulative<-cumulative+as.double(probabilities[[i]])
    selected<-c(selected,as.integer(i-1L))
    if (cumulative>=threshold) break
  }
  as.integer(selected)
}
