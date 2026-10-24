# Full-sequence causal scaled dot-product attention, one head.
causal_attention <- function(queries,keys,values) {
  valid_matrix <- function(x) is.matrix(x) && is.numeric(x) &&
    !is.complex(x) && nrow(x)>=1L && ncol(x)>=1L &&
    all(is.finite(x))
  if (!valid_matrix(queries) || !valid_matrix(keys) ||
      !valid_matrix(values) || nrow(keys)!=nrow(queries) ||
      ncol(keys)!=ncol(queries) || nrow(values)!=nrow(queries)) {
    stop("nonempty aligned finite query/key/value matrices required")
  }
  t<-nrow(queries);d<-ncol(queries);v<-ncol(values)
  output<-matrix(0.0,nrow=t,ncol=v)
  scores<-numeric(t)
  for (row in seq_len(t)) {
    maximum<--Inf
    for (j in seq_len(row)) {
      dot<-0.0
      for (i in seq_len(d)) {
        term<-as.double(queries[row,i])*as.double(keys[j,i])
        if (!is.finite(term)) stop("dot product overflow")
        dot<-dot+term
        if (!is.finite(dot)) stop("dot sum overflow")
      }
      scores[[j]]<-dot/sqrt(d)
      if (!is.finite(scores[[j]])) stop("attention score overflow")
      maximum<-max(maximum,scores[[j]])
    }
    denominator<-0.0
    for (j in seq_len(row)) {
      shifted<-scores[[j]]-maximum
      if (!is.finite(shifted)) stop("softmax shift overflow")
      scores[[j]]<-exp(shifted)
      denominator<-denominator+scores[[j]]
      if (!is.finite(denominator)) stop("softmax denominator overflow")
    }
    if (denominator<=0.0) stop("invalid softmax denominator")
    for (j in seq_len(row)) {
      weight<-scores[[j]]/denominator
      for (col in seq_len(v)) {
        term<-weight*as.double(values[j,col])
        if (!is.finite(term)) stop("weighted product overflow")
        output[row,col]<-output[row,col]+term
        if (!is.finite(output[row,col])) stop("output overflow")
      }
    }
  }
  output
}
