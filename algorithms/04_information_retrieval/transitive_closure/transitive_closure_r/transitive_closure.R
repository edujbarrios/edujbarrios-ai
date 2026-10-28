# Warshall reflexive reachability on a directed binary adjacency matrix.
transitive_closure <- function(adjacency) {
  if (!is.matrix(adjacency) || !is.numeric(adjacency) ||
      is.complex(adjacency) || nrow(adjacency)<1L ||
      nrow(adjacency)>64L || nrow(adjacency)!=ncol(adjacency) ||
      any(!is.finite(adjacency)) || any(!(adjacency %in% c(0,1)))) {
    stop("invalid square binary directed adjacency matrix")
  }
  n <- nrow(adjacency)
  reach <- matrix(as.integer(adjacency),nrow=n,ncol=n)
  diag(reach) <- 1L
  for (k in seq_len(n)) {
    for (i in seq_len(n)) {
      if (reach[i,k]==1L) {
        for (j in seq_len(n)) {
          if (reach[k,j]==1L) reach[i,j] <- 1L
        }
      }
    }
  }
  reach
}
