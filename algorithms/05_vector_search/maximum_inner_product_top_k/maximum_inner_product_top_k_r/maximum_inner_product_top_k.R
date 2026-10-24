# Exact maximum inner-product search: descending dot score, ascending row tie.
maximum_inner_product_top_k <- function(query,candidates,k) {
  if (!is.numeric(query) || is.complex(query) ||
      length(query)==0L || any(!is.finite(query)) ||
      !is.matrix(candidates) || !is.numeric(candidates) ||
      is.complex(candidates) || nrow(candidates)==0L ||
      ncol(candidates)!=length(query) || any(!is.finite(candidates)) ||
      !is.numeric(k) || is.complex(k) || length(k)!=1L ||
      !is.finite(k) || k!=floor(k) || k<1 || k>nrow(candidates)) {
    stop("invalid finite vector data, dimensions, or cutoff")
  }
  scores<-numeric(nrow(candidates))
  for (i in seq_len(nrow(candidates))) {
    score<-0.0
    for (j in seq_along(query)) {
      product<-as.double(query[[j]])*as.double(candidates[i,j])
      if (!is.finite(product)) stop("dot product overflow")
      score<-score+product
      if (!is.finite(score)) stop("dot accumulation overflow")
    }
    scores[[i]]<-score
  }
  as.integer(order(-scores,seq_along(scores))[seq_len(k)]-1L)
}
