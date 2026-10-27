# Zero-based history frequency penalty applied to next-token logits.
token_frequency_penalty <- function(logits, history, penalty) {
  if (!is.numeric(logits) || is.complex(logits) || !is.null(dim(logits)) ||
      length(logits)<1L || length(logits)>4096L ||
      any(!is.finite(logits)) || any(abs(logits)>1000) ||
      !is.numeric(history) || is.complex(history) || !is.null(dim(history)) ||
      length(history)>4096L || any(!is.finite(history)) ||
      any(history!=floor(history)) ||
      any(history<0) || any(history>=length(logits)) ||
      !is.numeric(penalty) || is.complex(penalty) ||
      !is.null(dim(penalty)) || length(penalty)!=1L ||
      !is.finite(penalty) || penalty<0 || penalty>5) {
    stop("invalid logits, history, or penalty")
  }
  counts <- tabulate(as.integer(history)+1L,nbins=length(logits))
  as.double(logits-penalty*counts)
}
