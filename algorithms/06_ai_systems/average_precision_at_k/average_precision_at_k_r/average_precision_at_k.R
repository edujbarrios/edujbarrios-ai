# Average Precision at K with capped total relevant normalization.
average_precision_at_k <- function(relevance,k,total_relevant) {
  scalar_integer<-function(v,low,high) {
    is.numeric(v) && !is.complex(v) && length(v)==1L &&
      is.finite(v) && v==floor(v) && v>=low && v<=high
  }
  if (!is.numeric(relevance) || is.complex(relevance) ||
      length(relevance)==0L || any(!is.finite(relevance)) ||
      any(!(relevance %in% c(0,1))) ||
      !scalar_integer(k,1,length(relevance)) ||
      !scalar_integer(total_relevant,0,2147483647) ||
      sum(relevance)>total_relevant) {
    stop("invalid relevance, k or corpus relevance count")
  }
  if (total_relevant==0) return(0.0)
  hits<-0.0
  weighted<-0.0
  for (i in seq_len(k)) {
    flag<-as.double(relevance[[i]])
    hits<-hits+flag
    if (flag==1) weighted<-weighted+hits/i
  }
  as.double(weighted/min(total_relevant,k))
}
