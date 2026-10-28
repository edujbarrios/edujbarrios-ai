# Deterministic indegree-deletion topological order for a directed binary DAG.
topological_sort <- function(weights) {
  if (!is.matrix(weights) || !is.numeric(weights) || is.complex(weights) ||
      nrow(weights)<1L || nrow(weights)>128L ||
      nrow(weights)!=ncol(weights) || any(!is.finite(weights)) ||
      any(!(weights %in% c(0,1)))) {
    stop("invalid square binary adjacency matrix")
  }
  n <- nrow(weights)
  indegree <- as.integer(colSums(weights))
  used <- rep(FALSE,n)
  result <- integer(n)
  for (k in seq_len(n)) {
    candidates <- which(!used & indegree==0L)
    if (length(candidates)==0L) stop("directed cycle detected")
    u <- candidates[[1L]]
    result[[k]] <- u-1L
    used[[u]] <- TRUE
    indegree <- indegree-as.integer(weights[u,])
  }
  result
}
