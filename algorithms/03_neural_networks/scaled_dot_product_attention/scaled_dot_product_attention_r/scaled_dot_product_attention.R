# Stable single-query scaled dot-product attention.
scaled_dot_product_attention <- function(query, keys, values) {
  if (!is.numeric(query) || is.complex(query) || length(query)<1L ||
      any(!is.finite(query)) || !is.matrix(keys) ||
      !is.numeric(keys) || is.complex(keys) || nrow(keys)<1L ||
      ncol(keys)!=length(query) || any(!is.finite(keys)) ||
      !is.matrix(values) || !is.numeric(values) || is.complex(values) ||
      nrow(values)!=nrow(keys) || ncol(values)<1L ||
      any(!is.finite(values))) {
    stop("invalid query, keys or values")
  }
  m<-nrow(keys);d<-length(query);v<-ncol(values)
  scores<-numeric(m)
  maximum<--Inf
  for (j in seq_len(m)) {
    dot<-0.0
    for (i in seq_len(d)) {
      product<-as.double(query[[i]])*as.double(keys[j,i])
      if (!is.finite(product)) stop("dot product overflow")
      dot<-dot+product
      if (!is.finite(dot)) stop("dot sum overflow")
    }
    score<-dot/sqrt(d)
    if (!is.finite(score)) stop("score overflow")
    scores[[j]]<-score
    maximum<-max(maximum,score)
  }
  exponents<-numeric(m)
  denominator<-0.0
  for (j in seq_len(m)) {
    delta<-scores[[j]]-maximum
    if (!is.finite(delta)) stop("softmax shift overflow")
    exponents[[j]]<-exp(delta)
    denominator<-denominator+exponents[[j]]
    if (!is.finite(denominator)) stop("softmax denominator overflow")
  }
  if (denominator<=0) stop("invalid softmax denominator")
  result<-numeric(v)
  for (j in seq_len(m)) {
    weight<-exponents[[j]]/denominator
    for (k in seq_len(v)) {
      term<-weight*as.double(values[j,k])
      if (!is.finite(term)) stop("weighted product overflow")
      result[[k]]<-result[[k]]+term
      if (!is.finite(result[[k]])) stop("weighted sum overflow")
    }
  }
  result
}
