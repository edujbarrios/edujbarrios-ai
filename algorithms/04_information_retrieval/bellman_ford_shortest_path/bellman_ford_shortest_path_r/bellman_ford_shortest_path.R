# Bellman-Ford directed graph distances; vertex IDs in all languages are zero-based.
bellman_ford_shortest_path <- function(edges, n, source) {
  if (!is.numeric(n) || is.complex(n) || length(n)!=1L ||
      !is.finite(n) || n!=floor(n) || n<1 || n>128 ||
      !is.numeric(source) || is.complex(source) || length(source)!=1L ||
      !is.finite(source) || source!=floor(source) || source<0 || source>=n ||
      !is.matrix(edges) || !is.numeric(edges) || is.complex(edges) ||
      ncol(edges)!=3L || nrow(edges)>16384L ||
      any(!is.finite(edges)) || any(abs(edges[,3L])>1000000) ||
      any(edges[,1:2,drop=FALSE] != floor(edges[,1:2,drop=FALSE])) ||
      any(edges[,1:2,drop=FALSE]<0) ||
      any(edges[,1:2,drop=FALSE]>=n)) {
    stop("invalid graph, vertex index or edge weight")
  }
  n <- as.integer(n)
  source <- as.integer(source)
  dist <- rep(Inf, n)
  dist[[source+1L]] <- 0.0
  if (n>1L) {
    for (pass in seq_len(n-1L)) {
      changed <- FALSE
      if (nrow(edges)>0L) for (i in seq_len(nrow(edges))) {
        u <- as.integer(edges[i,1L])+1L
        v <- as.integer(edges[i,2L])+1L
        if (!is.finite(dist[[u]])) next
        candidate <- dist[[u]]+edges[i,3L]
        if (!is.finite(candidate)) stop("distance overflow")
        if (candidate<dist[[v]]) {
          dist[[v]] <- candidate
          changed <- TRUE
        }
      }
      if (!changed) break
    }
  }
  if (nrow(edges)>0L) for (i in seq_len(nrow(edges))) {
    u <- as.integer(edges[i,1L])+1L
    v <- as.integer(edges[i,2L])+1L
    if (is.finite(dist[[u]]) && dist[[u]]+edges[i,3L]<dist[[v]])
      stop("reachable negative cycle")
  }
  as.double(dist)
}
