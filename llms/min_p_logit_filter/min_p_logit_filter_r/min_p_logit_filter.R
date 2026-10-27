# Min-p relative probability threshold: keep exp(logit-max) >= min_p.
min_p_logit_filter <- function(logits, min_p) {
  if (!is.numeric(logits) || is.complex(logits) ||
      !is.null(dim(logits)) || length(logits)<1L ||
      length(logits)>4096L || any(!is.finite(logits)) ||
      any(abs(logits)>1000) || !is.numeric(min_p) ||
      is.complex(min_p) || !is.null(dim(min_p)) ||
      length(min_p)!=1L || !is.finite(min_p) ||
      min_p<0 || min_p>1) {
    stop("invalid logits or min_p")
  }
  if (min_p==0) return(rep(TRUE,length(logits)))
  as.logical(exp(logits-max(logits))>=min_p)
}
