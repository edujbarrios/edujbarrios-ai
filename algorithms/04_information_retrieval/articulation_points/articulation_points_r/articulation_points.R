# Tarjan low-link articulation points for an undirected simple graph.
articulation_points <- function(adjacency,n) {
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
    if (adjacency[[(u-1L)*n+u]]!=0) stop("self-loops are unsupported")
    for (v in seq_len(u-1L)) {
      if (adjacency[[(u-1L)*n+v]]!=adjacency[[(v-1L)*n+u]]) {
        stop("adjacency matrix must be symmetric")
      }
    }
  }
  disc <- integer(n)
  low <- integer(n)
  parent <- integer(n)
  cut <- rep.int(FALSE,n)
  clock <- 0L
  dfs <- function(u) {
    clock <<- clock+1L
    disc[[u]] <<- clock
    low[[u]] <<- clock
    children <- 0L
    for (v in seq_len(n)) {
      if (adjacency[[(u-1L)*n+v]]==0) next
      if (disc[[v]]==0L) {
        parent[[v]] <<- u
        children <- children+1L
        dfs(v)
        low[[u]] <<- min(low[[u]],low[[v]])
        if (parent[[u]]!=0L && low[[v]]>=disc[[u]]) cut[[u]] <<- TRUE
      } else if (v!=parent[[u]]) {
        low[[u]] <<- min(low[[u]],disc[[v]])
      }
    }
    if (parent[[u]]==0L && children>1L) cut[[u]] <<- TRUE
  }
  for (u in seq_len(n)) if (disc[[u]]==0L) dfs(u)
  as.integer(which(cut)-1L)
}
