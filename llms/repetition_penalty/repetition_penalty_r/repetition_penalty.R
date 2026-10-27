# Sign-aware repetition penalty on flat vocabulary logits; zero-based token IDs.
repetition_penalty <- function(logits, history, penalty) {
  if (!is.numeric(logits) || is.complex(logits) ||
      !is.null(dim(logits)) || length(logits)<1L ||
      length(logits)>4096L || any(!is.finite(logits)) ||
      any(abs(logits)>1000) || !is.numeric(history) ||
      is.complex(history) || !is.null(dim(history)) ||
      length(history)>16384L || any(!is.finite(history)) ||
      any(history!=floor(history)) || any(history<0) ||
      any(history>=length(logits)) || !is.numeric(penalty) ||
      is.complex(penalty) || length(penalty)!=1L ||
      !is.finite(penalty) || penalty<1 || penalty>10) {
    stop("invalid logits, history, or penalty")
  }
  out <- as.double(logits)
  if (length(history)>0L) {
    ids <- unique(as.integer(history))+1L
    positive <- ids[out[ids]>0]
    other <- ids[out[ids]<=0]
    out[positive] <- out[positive]/penalty
    out[other] <- out[other]*penalty
  }
  out
}
