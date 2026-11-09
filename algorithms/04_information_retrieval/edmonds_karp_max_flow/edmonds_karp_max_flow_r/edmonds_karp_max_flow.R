# Edmonds-Karp maximum flow on a small directed integral-capacity graph.
edmonds_karp_max_flow <- function(capacities, n, source, sink) {
  valid_index <- function(x) {
    is.numeric(x) && !is.logical(x) && !is.complex(x) &&
      is.null(dim(x)) && length(x)==1L && is.finite(x) && x==floor(x)
  }
  if (!valid_index(n) || n<2 || n>16 ||
      !valid_index(source) || !valid_index(sink) ||
      source<0 || sink<0 || source>=n || sink>=n || source==sink) {
    stop("invalid graph size or source/sink")
  }
  n <- as.integer(n)
  source <- as.integer(source)+1L
  sink <- as.integer(sink)+1L
  if (!is.numeric(capacities) || is.logical(capacities) ||
      is.complex(capacities) || !is.null(dim(capacities)) ||
      length(capacities)!=n*n || any(!is.finite(capacities)) ||
      any(capacities<0 | capacities>1000) ||
      any(capacities!=floor(capacities))) {
    stop("invalid directed capacity matrix")
  }
  residual <- as.integer(capacities)
  for (u in seq_len(n)) if (residual[[(u-1L)*n+u]]!=0L) {
    stop("self-loop capacity unsupported")
  }
  result <- 0L
  repeat {
    parent <- integer(n)
    parent[[source]] <- source
    queue <- integer(n)
    head <- 1L
    tail <- 1L
    queue[[1L]] <- source
    while (head<=tail && parent[[sink]]==0L) {
      u <- queue[[head]]
      head <- head+1L
      for (v in seq_len(n)) {
        if (parent[[v]]==0L && residual[[(u-1L)*n+v]]>0L) {
          parent[[v]] <- u
          tail <- tail+1L
          queue[[tail]] <- v
        }
      }
    }
    if (parent[[sink]]==0L) return(as.integer(result))
    augment <- 1000L
    v <- sink
    while (v!=source) {
      u <- parent[[v]]
      augment <- min(augment,residual[[(u-1L)*n+v]])
      v <- u
    }
    v <- sink
    while (v!=source) {
      u <- parent[[v]]
      residual[[(u-1L)*n+v]] <- residual[[(u-1L)*n+v]]-augment
      residual[[(v-1L)*n+u]] <- residual[[(v-1L)*n+u]]+augment
      v <- u
    }
    result <- result+augment
  }
}
