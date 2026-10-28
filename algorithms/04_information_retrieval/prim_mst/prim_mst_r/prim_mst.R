# Dense Prim MST cost with zero meaning no edge.
prim_mst <- function(weights) {
  if (!is.matrix(weights) || !is.numeric(weights) || is.complex(weights) ||
      nrow(weights)<1L || nrow(weights)>128L ||
      nrow(weights)!=ncol(weights) ||
      any(!is.finite(weights)) || any(weights<0) ||
      any(weights>1000000) ||
      any(diag(weights)!=0) || any(weights!=t(weights))) {
    stop("invalid symmetric adjacency matrix")
  }
  n <- nrow(weights)
  inside <- rep(FALSE,n)
  best <- rep(Inf,n)
  best[[1L]] <- 0
  total <- 0.0
  for (step in seq_len(n)) {
    candidates <- which(!inside & is.finite(best))
    if (length(candidates)==0L) stop("graph is disconnected")
    u <- candidates[[which.min(best[candidates])]]
    inside[[u]] <- TRUE
    total <- total+best[[u]]
    for (v in seq_len(n)) {
      w <- as.double(weights[u,v])
      if (!inside[[v]] && w>0 && w<best[[v]]) best[[v]] <- w
    }
  }
  as.double(total)
}
