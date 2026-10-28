# Undirected graph bipartiteness using BFS two-coloring.
bipartite_graph_check <- function(adjacency) {
  if (!is.matrix(adjacency) || !is.numeric(adjacency) ||
      is.complex(adjacency) || nrow(adjacency)<1L ||
      nrow(adjacency)>128L || nrow(adjacency)!=ncol(adjacency) ||
      any(!is.finite(adjacency)) || any(!(adjacency %in% c(0,1))) ||
      !all(adjacency==t(adjacency))) {
    stop("symmetric binary adjacency matrix of size 1..128 required")
  }
  n<-nrow(adjacency)
  color<-rep.int(-1L,n)
  for (start in seq_len(n)) {
    if (color[[start]]!=-1L) next
    queue<-integer(n)
    head<-1L
    tail<-1L
    queue[[tail]]<-start
    color[[start]]<-0L
    while (head<=tail) {
      u<-queue[[head]]
      head<-head+1L
      for (v in seq_len(n)) {
        if (adjacency[u,v]==0) next
        if (color[[v]]==-1L) {
          color[[v]]<-1L-color[[u]]
          tail<-tail+1L
          queue[[tail]]<-v
        } else if (color[[v]]==color[[u]]) return(FALSE)
      }
    }
  }
  TRUE
}
