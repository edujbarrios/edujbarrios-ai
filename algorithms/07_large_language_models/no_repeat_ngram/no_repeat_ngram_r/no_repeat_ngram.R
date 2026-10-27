# Boolean mask for next tokens that would repeat an existing n-gram.
no_repeat_ngram <- function(history, vocabulary_size, n) {
  scalar_int <- function(x, minimum, maximum) {
    is.numeric(x) && !is.complex(x) && length(x)==1L &&
      is.null(dim(x)) && is.finite(x) && x==floor(x) &&
      x>=minimum && x<=maximum
  }
  if (!scalar_int(vocabulary_size,1,4096) ||
      !scalar_int(n,1,32) ||
      !is.numeric(history) || is.complex(history) ||
      !is.null(dim(history)) || length(history)>16384L ||
      any(!is.finite(history)) || any(history!=floor(history)) ||
      any(history<0) || any(history>=vocabulary_size)) {
    stop("invalid n-gram mask parameters or token IDs")
  }
  vocabulary_size <- as.integer(vocabulary_size)
  n <- as.integer(n)
  length_history <- length(history)
  blocked <- rep(FALSE,vocabulary_size)
  if (length_history<n) return(blocked)
  if (n==1L) {
    blocked[as.integer(history)+1L] <- TRUE
    return(blocked)
  }
  suffix <- history[(length_history-n+2L):length_history]
  for (i in seq_len(length_history-n+1L)) {
    if (all(history[i:(i+n-2L)]==suffix)) {
      blocked[[as.integer(history[[i+n-1L]])+1L]] <- TRUE
    }
  }
  blocked
}
