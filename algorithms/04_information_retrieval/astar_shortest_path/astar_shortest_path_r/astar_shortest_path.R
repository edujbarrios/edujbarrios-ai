# Dense A* graph search with a consistent nonnegative node heuristic.
astar_shortest_path <- function(weights,heuristic,source,target) {
  if (!is.matrix(weights) || !is.numeric(weights) || is.complex(weights) ||
      nrow(weights)<1L || nrow(weights)>64L ||
      ncol(weights)!=nrow(weights) ||
      any(!is.finite(weights)) || any(weights<0) || any(weights>1000000) ||
      !is.numeric(heuristic) || is.complex(heuristic) ||
      length(heuristic)!=nrow(weights) ||
      any(!is.finite(heuristic)) || any(heuristic<0) ||
      any(heuristic>1000000) ||
      !is.numeric(source) || is.complex(source) || length(source)!=1L ||
      !is.finite(source) || source!=floor(source) ||
      !is.numeric(target) || is.complex(target) || length(target)!=1L ||
      !is.finite(target) || target!=floor(target) ||
      source<0 || source>=nrow(weights) || target<0 || target>=nrow(weights)) {
    stop("invalid graph weights, heuristic or node IDs")
  }
  n<-nrow(weights);s<-as.integer(source)+1L;t<-as.integer(target)+1L
  if (heuristic[[t]]!=0) stop("target heuristic must be zero")
  for (u in seq_len(n)) {
    for (v in seq_len(n)) {
      weight<-as.double(weights[u,v])
      if (weight>0 && heuristic[[u]]>weight+heuristic[[v]]+1e-9) {
        stop("inconsistent A-star heuristic")
      }
    }
  }
  g<-rep(Inf,n)
  previous<-rep(NA_integer_,n)
  closed<-rep(FALSE,n)
  g[[s]]<-0.0
  for (step in seq_len(n)) {
    selected<-NA_integer_
    best<-Inf
    for (i in seq_len(n)) {
      if (closed[[i]] || !is.finite(g[[i]])) next
      priority<-g[[i]]+heuristic[[i]]
      if (!is.finite(priority)) stop("A-star priority overflow")
      if (is.na(selected) || priority<best) {
        selected<-as.integer(i);best<-priority
      }
    }
    if (is.na(selected)) return(list(distance=-1.0,path=integer()))
    if (selected==t) {
      path<-as.integer(t-1L)
      current<-selected
      while (current!=s) {
        current<-previous[[current]]
        if (is.na(current) || length(path)>=n) stop("broken predecessor path")
        path<-c(path,as.integer(current-1L))
      }
      return(list(distance=as.double(g[[t]]),path=rev(as.integer(path))))
    }
    closed[[selected]]<-TRUE
    for (v in seq_len(n)) {
      weight<-as.double(weights[selected,v])
      if (weight==0 || closed[[v]]) next
      tentative<-g[[selected]]+weight
      if (!is.finite(tentative)) stop("A-star cost overflow")
      if (tentative<g[[v]]) {
        g[[v]]<-tentative
        previous[[v]]<-selected
      }
    }
  }
  list(distance=-1.0,path=integer())
}
