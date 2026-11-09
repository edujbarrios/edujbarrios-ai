# Inverse-distance-weighted KNN regression with exact-match averaging.
weighted_knn_regression <- function(train,targets,query,k) {
  valid_vector <- function(v,length_needed) {
    is.numeric(v) && !is.logical(v) && !is.complex(v) &&
      is.null(dim(v)) && length(v)==length_needed &&
      all(is.finite(v)) && all(abs(v)<=1000)
  }
  if (!is.matrix(train) || !is.numeric(train) || is.logical(train) ||
      is.complex(train) || nrow(train)<1L || nrow(train)>128L ||
      ncol(train)<1L || ncol(train)>16L ||
      any(!is.finite(train)) || any(abs(train)>1000) ||
      !valid_vector(targets,nrow(train)) ||
      !valid_vector(query,ncol(train)) ||
      !is.numeric(k) || is.logical(k) || is.complex(k) ||
      !is.null(dim(k)) || length(k)!=1L || !is.finite(k) ||
      k!=floor(k) || k<1 || k>nrow(train)) {
    stop("invalid numeric training matrix, targets, query or k")
  }
  k <- as.integer(k)
  n <- nrow(train)
  d <- ncol(train)
  distances <- numeric(n)
  for (i in seq_len(n)) {
    norm <- 0.0
    for (j in seq_len(d)) {
      delta <- as.double(train[i,j])-as.double(query[[j]])
      scale <- max(norm,abs(delta))
      norm <- if (scale==0) 0.0 else
        scale*sqrt((norm/scale)^2+(delta/scale)^2)
    }
    distances[[i]] <- norm
  }
  chosen <- order(distances,seq_len(n))[seq_len(k)]
  selected <- distances[chosen]
  zeros <- chosen[selected==0]
  if (length(zeros)>0L) return(as.double(sum(targets[zeros]/length(zeros))))
  weights <- min(selected)/selected
  result <- sum(weights*targets[chosen])/sum(weights)
  if (!is.finite(result)) stop("nonfinite prediction")
  as.double(result)
}
