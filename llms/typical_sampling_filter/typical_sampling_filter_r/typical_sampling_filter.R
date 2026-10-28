# Locally typical decoding candidate mask, output in original ID order.
typical_sampling_filter <- function(logits, p) {
  if (!is.numeric(logits) || is.complex(logits) || !is.null(dim(logits)) ||
      length(logits)<1L || length(logits)>4096L ||
      any(!is.finite(logits)) || any(abs(logits)>1000) ||
      !is.numeric(p) || is.complex(p) || !is.null(dim(p)) ||
      length(p)!=1L || !is.finite(p) || p<=0 || p>1) {
    stop("invalid logits or typical threshold")
  }
  w <- exp(logits-max(logits))
  total <- sum(w)
  surprise <- log(total)-(logits-max(logits))
  entropy <- sum((w/total)*surprise)
  order_ids <- order(abs(surprise-entropy),seq_along(logits))
  keep <- rep(FALSE,length(logits))
  mass <- 0.0
  for (i in order_ids) {
    keep[[i]] <- TRUE
    mass <- mass+w[[i]]/total
    if (p<1 && mass>=p) break
  }
  keep
}
