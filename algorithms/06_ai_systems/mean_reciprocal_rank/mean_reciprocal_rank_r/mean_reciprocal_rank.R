# Mean reciprocal rank, where zero indicates no relevant hit.
mean_reciprocal_rank <- function(ranks) {
  if (!is.numeric(ranks) || is.complex(ranks) || length(ranks)==0L ||
      any(!is.finite(ranks)) || any(ranks < 0) ||
      any(ranks != floor(ranks)) || any(ranks > 2^53)) {
    stop("ranks must be nonempty, exact nonnegative integers")
  }
  contributions <- numeric(length(ranks))
  hits <- ranks > 0
  contributions[hits] <- 1 / as.double(ranks[hits])
  score <- sum(contributions) / length(ranks)
  if (!is.finite(score) || score < 0 || score > 1) stop("invalid MRR")
  as.double(score)
}
