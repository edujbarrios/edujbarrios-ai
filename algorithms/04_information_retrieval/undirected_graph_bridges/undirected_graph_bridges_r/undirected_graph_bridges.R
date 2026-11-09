# Undirected graph bridge endpoints via DFS discovery and low-link times.
undirected_graph_bridges <- function(adjacency,n) {
  valid_n <- is.numeric(n) && !is.logical(n) && !is.complex(n) &&
    is.null(dim(n)) && length(n)==1L && is.finite(n) &&
    n==floor(n) && n>=1 && n<=64
  if (!valid_n) stop("vertex count must be integer 1..64")
  n <- as.integer(n)
  if (!is.numeric(adjacency) || is.logical(adjacency) ||
      is.complex(adjacency) || !is.null(dim(adjacency)) ||
      length(adjacency)!=n*n || any(!is.finite(adjacency)) ||
      any(!(adjacency %in% c(0,1)))) stop("invalid binary adjacency matrix")
  for (u in seq_len(n)) {
    if (adjacency[[(u-1L)*n+u]]!=0) stop("self-loops unsupported")
    if (u>1L) for(v in seq_len(u-1L)) {
      if (adjacency[[(u-1L)*n+v]]!=adjacency[[(v-1L)*n+u]]) {
        stop("adjacency must be symmetric")
      }
    }
  }
  disc <- integer(n)
  low <- integer(n)
  parent <- integer(n)
  marked <- rep.int(FALSE,n*n)
  clock <- 0L
  dfs <- function(u) {
    clock <<- clock+1L
    disc[[u]] <<- clock
    low[[u]] <<- clock
    for (v in seq_len(n)) {
      if (adjacency[[(u-1L)*n+v]]==0) next
      if (disc[[v]]==0L) {
        parent[[v]] <<- u
        dfs(v)
        low[[u]] <<- min(low[[u]],low[[v]])
        if (low[[v]]>disc[[u]]) {
          lo <- min(u,v)
          hi <- max(u,v)
          marked[[(lo-1L)*n+hi]] <<- TRUE
        }
      } else if (v!=parent[[u]]) {
        low[[u]] <<- min(low[[u]],disc[[v]])
      }
    }
  }
  for (u in seq_len(n)) if (disc[[u]]==0L) dfs(u)
  result <- integer(0)
  for (u in seq_len(n)) if (u<n) for (v in (u+1L):n) {
    if (marked[[(u-1L)*n+v]]) result <- c(result,u-1L,v-1L)
  }
  as.integer(result)
}
