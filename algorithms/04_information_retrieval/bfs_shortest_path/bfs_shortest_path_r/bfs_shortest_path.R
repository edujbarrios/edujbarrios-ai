# Breadth-first search hop distances on a directed binary adjacency matrix.
bfs_shortest_path <- function(weights, source) {
  if (!is.matrix(weights) || !is.numeric(weights) || is.complex(weights) ||
      nrow(weights)<1L || nrow(weights)>128L || nrow(weights)!=ncol(weights) ||
      any(!is.finite(weights)) || any(!(weights %in% c(0,1))) ||
      !is.numeric(source) || is.complex(source) || length(source)!=1L ||
      !is.finite(source) || source!=floor(source) ||
      source<0 || source>=nrow(weights)) {
    stop("invalid directed binary matrix or source")
  }
  n <- nrow(weights)
  distance <- rep.int(-1L,n)
  queue <- integer(n)
  head <- 1L
  tail <- 1L
  start <- as.integer(source)+1L
  queue[[tail]] <- start
  distance[[start]] <- 0L
  while (head<=tail) {
    u <- queue[[head]]
    head <- head+1L
    for (v in seq_len(n)) {
      if (weights[u,v]==1 && distance[[v]]==-1L) {
        distance[[v]] <- distance[[u]]+1L
        tail <- tail+1L
        queue[[tail]] <- v
      }
    }
  }
  as.integer(distance)
}
