# Nucleus keep mask: stable softmax then score-descending prefix.
top_p_logit_filter <- function(logits, top_p) {
  if (!is.numeric(logits) || is.complex(logits) ||
      !is.null(dim(logits)) || length(logits)<1L || length(logits)>4096L ||
      any(!is.finite(logits)) || any(abs(logits)>1000) ||
      !is.numeric(top_p) || is.complex(top_p) ||
      length(top_p)!=1L || !is.null(dim(top_p)) ||
      !is.finite(top_p) || top_p<=0 || top_p>1) {
    stop("invalid logits or top_p")
  }
  weights <- exp(logits-max(logits))
  weights <- weights/sum(weights)
  indices <- order(-logits,seq_along(logits))
  keep <- rep(FALSE,length(logits))
  cumulative <- 0.0
  for (i in indices) {
    keep[[i]] <- TRUE
    cumulative <- cumulative + weights[[i]]
    if (top_p<1 && cumulative>=top_p) break
  }
  keep
}
