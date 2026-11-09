# Exact exhaustive Euclidean radius search over numeric matrix rows.
euclidean_radius_search <- function(query,candidates,radius) {
  if (!is.numeric(query) || is.logical(query) || is.complex(query) ||
      !is.null(dim(query)) || length(query)<1L || length(query)>32L ||
      any(!is.finite(query)) || any(abs(query)>1000) ||
      !is.matrix(candidates) || !is.numeric(candidates) ||
      is.logical(candidates) || is.complex(candidates) ||
      nrow(candidates)<1L || nrow(candidates)>128L ||
      ncol(candidates)!=length(query) || any(!is.finite(candidates)) ||
      any(abs(candidates)>1000) ||
      !is.numeric(radius) || is.logical(radius) || is.complex(radius) ||
      !is.null(dim(radius)) || length(radius)!=1L || !is.finite(radius) ||
      radius<0 || radius>10000) {
    stop("invalid query, candidate matrix or radius")
  }
  distances <- numeric(nrow(candidates))
  for (i in seq_len(nrow(candidates))) {
    d <- 0.0
    for (j in seq_along(query)) {
      delta <- as.double(query[[j]])-as.double(candidates[i,j])
      d <- if (d==0 && delta==0) 0 else {
        scale <- max(d,abs(delta))
        scale*sqrt((d/scale)^2+(delta/scale)^2)
      }
    }
    distances[[i]] <- d
  }
  indices <- which(distances<=radius)
  if (length(indices)==0L) return(integer(0))
  as.integer(indices[order(distances[indices],indices)]-1L)
}
