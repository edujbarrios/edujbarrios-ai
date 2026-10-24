# Exact cosine top-k ranking of dense numeric candidate rows.
exact_cosine_top_k <- function(query, candidates, k) {
  if (!is.numeric(query) || is.complex(query) ||
      length(query)==0L || any(!is.finite(query)) ||
      !is.matrix(candidates) || !is.numeric(candidates) ||
      is.complex(candidates) || nrow(candidates)==0L ||
      ncol(candidates)!=length(query) ||
      any(!is.finite(candidates)) ||
      !is.numeric(k) || is.complex(k) ||
      length(k)!=1L || !is.finite(k) || k!=floor(k) ||
      k<1 || k>nrow(candidates)) {
    stop("invalid dimensions, finite data, or k")
  }
  magnitude <- function(v) {
    norm <- 0.0
    for (value in v) {
      scale <- max(norm, abs(value))
      if (scale == 0) {
        norm <- 0.0
      } else {
        norm <- scale * sqrt((norm/scale)^2 + (value/scale)^2)
      }
      if (!is.finite(norm)) stop("norm overflow")
    }
    if (norm<=0) stop("zero vector")
    norm
  }
  qnorm <- magnitude(as.double(query))
  scores <- numeric(nrow(candidates))
  for (j in seq_len(nrow(candidates))) {
    row <- as.double(candidates[j,])
    vnorm <- magnitude(row)
    score <- 0.0
    for (i in seq_along(query)) {
      term <- (as.double(query[[i]])/qnorm)*(row[[i]]/vnorm)
      score <- score + term
      if (any(!is.finite(c(term,score)))) stop("cosine score overflow")
    }
    scores[[j]] <- max(-1.0,min(1.0,score))
  }
  as.integer(order(-scores,seq_along(scores))[seq_len(k)]-1L)
}
