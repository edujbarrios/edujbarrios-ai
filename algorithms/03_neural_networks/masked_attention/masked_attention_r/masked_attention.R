# Stable masked single-query attention: 1 means allowed, 0 excluded.
masked_attention <- function(query, keys, values, mask) {
  if (!is.numeric(query) || is.complex(query) ||
      length(query)==0L || any(!is.finite(query)) ||
      !is.matrix(keys) || !is.numeric(keys) || is.complex(keys) ||
      nrow(keys)<1L || ncol(keys)!=length(query) ||
      any(!is.finite(keys)) ||
      !is.matrix(values) || !is.numeric(values) || is.complex(values) ||
      nrow(values)!=nrow(keys) || ncol(values)<1L ||
      any(!is.finite(values)) ||
      !is.numeric(mask) || is.complex(mask) ||
      length(mask)!=nrow(keys) || any(!is.finite(mask)) ||
      any(!(mask %in% c(0,1))) || !any(mask==1)) {
    stop("invalid query, matrices or binary mask")
  }
  m<-nrow(keys); d<-length(query); v<-ncol(values)
  scores<-numeric(m)
  maximum<--Inf
  for (j in seq_len(m)) {
    if (mask[[j]]==0) next
    dot<-0.0
    for (i in seq_len(d)) {
      product<-as.double(query[[i]])*as.double(keys[j,i])
      if (!is.finite(product)) stop("dot product overflow")
      dot<-dot+product
      if (!is.finite(dot)) stop("dot sum overflow")
    }
    scores[[j]]<-dot/sqrt(d)
    if (!is.finite(scores[[j]])) stop("score overflow")
    maximum<-max(maximum,scores[[j]])
  }
  denominator<-0.0
  for (j in seq_len(m)) {
    if (mask[[j]]==0) next
    shifted<-scores[[j]]-maximum
    if (!is.finite(shifted)) stop("softmax shift overflow")
    scores[[j]]<-exp(shifted)
    denominator<-denominator+scores[[j]]
    if (!is.finite(denominator)) stop("softmax denominator overflow")
  }
  if (denominator<=0) stop("invalid softmax denominator")
  output<-numeric(v)
  for (j in seq_len(m)) {
    if (mask[[j]]==0) next
    weight<-scores[[j]]/denominator
    for (i in seq_len(v)) {
      term<-weight*as.double(values[j,i])
      if (!is.finite(term)) stop("weighted product overflow")
      output[[i]]<-output[[i]]+term
      if (!is.finite(output[[i]])) stop("output overflow")
    }
  }
  output
}
