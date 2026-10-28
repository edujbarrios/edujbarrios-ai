# Undirected components of a symmetric binary adjacency matrix.
undirected_connected_components <- function(adjacency) {
  if (!is.matrix(adjacency) || !is.numeric(adjacency) ||
      is.complex(adjacency) || nrow(adjacency)<1L ||
      nrow(adjacency)>128L || nrow(adjacency)!=ncol(adjacency) ||
      any(!is.finite(adjacency)) || any(!(adjacency %in% c(0,1))) ||
      !all(adjacency==t(adjacency))) {
    stop("undirected symmetric binary adjacency matrix required")
  }
  n <- nrow(adjacency)
  labels <- rep.int(-1L,n)
  for (start in seq_len(n)) {
    if (labels[[start]]!=-1L) next
    queue <- integer(n)
    head <- 1L
    tail <- 1L
    queue[[tail]] <- start
    labels[[start]] <- as.integer(start-1L)
    while (head<=tail) {
      u <- queue[[head]]
      head <- head+1L
      for (v in seq_len(n)) {
        if (adjacency[u,v]==1 && labels[[v]]==-1L) {
          labels[[v]] <- as.integer(start-1L)
          tail <- tail+1L
          queue[[tail]] <- v
        }
      }
    }
  }
  labels
}
