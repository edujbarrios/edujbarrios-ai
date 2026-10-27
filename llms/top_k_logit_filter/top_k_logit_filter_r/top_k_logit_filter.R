# Boolean keep mask for the k highest next-token logits (ID-tie order).
top_k_logit_filter <- function(logits, k) {
  if (!is.numeric(logits) || is.complex(logits) ||
      !is.null(dim(logits)) || length(logits)<1L || length(logits)>4096L ||
      any(!is.finite(logits)) || any(abs(logits)>1000) ||
      !is.numeric(k) || is.complex(k) || length(k)!=1L ||
      !is.null(dim(k)) || !is.finite(k) || k!=floor(k) ||
      k<1L || k>length(logits)) {
    stop("invalid logits or k")
  }
  order_indices <- order(-logits,seq_along(logits))
  keep <- rep(FALSE,length(logits))
  keep[order_indices[seq_len(as.integer(k))]] <- TRUE
  keep
}
