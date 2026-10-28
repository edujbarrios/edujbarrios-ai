# Kosaraju SCC decomposition with minimum-vertex canonical labels.
strongly_connected_components <- function(adjacency) {
  if (!is.matrix(adjacency) || !is.numeric(adjacency) ||
      is.complex(adjacency) || nrow(adjacency)<1L ||
      nrow(adjacency)>128L || nrow(adjacency)!=ncol(adjacency) ||
      any(!is.finite(adjacency)) || any(!(adjacency %in% c(0,1)))) {
    stop("invalid directed binary adjacency matrix")
  }
  n <- nrow(adjacency)
  seen <- rep(FALSE,n)
  order <- integer(0L)
  finish <- function(u) {
    seen[[u]] <<- TRUE
    for (v in seq_len(n)) {
      if (adjacency[u,v]==1 && !seen[[v]]) finish(v)
    }
    order <<- c(order,as.integer(u))
  }
  for (u in seq_len(n)) if (!seen[[u]]) finish(u)
  group <- rep.int(0L,n)
  collect <- function(u,root) {
    group[[u]] <<- as.integer(root)
    for (v in seq_len(n)) {
      if (adjacency[v,u]==1 && group[[v]]==0L) collect(v,root)
    }
  }
  for (u in rev(order)) if (group[[u]]==0L) collect(u,u)
  minima <- rep.int(n,n)
  for (u in seq_len(n)) minima[[group[[u]]]] <- min(minima[[group[[u]]]],u-1L)
  as.integer(minima[group])
}
