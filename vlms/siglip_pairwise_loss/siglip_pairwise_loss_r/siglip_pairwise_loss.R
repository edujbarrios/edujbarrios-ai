# SigLIP-style normalized pairwise logistic contrastive mean over N^2 pairs.
siglip_pairwise_loss <- function(images, texts, n, dimension, logit_scale, bias) {
  valid_dim <- function(value, low, high) {
    is.numeric(value) && !is.complex(value) && length(value)==1L &&
      is.null(dim(value)) && is.finite(value) &&
      value==floor(value) && value>=low && value<=high
  }
  valid_scalar <- function(value, low, high) {
    is.numeric(value) && !is.complex(value) && length(value)==1L &&
      is.null(dim(value)) && is.finite(value) &&
      value>=low && value<=high
  }
  if (!valid_dim(n,2,64) || !valid_dim(dimension,1,128) ||
      !valid_scalar(logit_scale,0,100) || logit_scale<=0 ||
      !valid_scalar(bias,-20,20) ||
      !is.numeric(images) || is.complex(images) ||
      !is.numeric(texts) || is.complex(texts) ||
      !is.null(dim(images)) || !is.null(dim(texts)) ||
      length(images)!=n*dimension || length(texts)!=n*dimension ||
      any(!is.finite(images)) || any(!is.finite(texts)) ||
      any(abs(images)>1000) || any(abs(texts)>1000)) {
    stop("invalid dimensions, vectors or loss parameters")
  }
  n <- as.integer(n)
  dimension <- as.integer(dimension)
  normalize <- function(values) {
    out <- matrix(0,nrow=n,ncol=dimension)
    for (i in seq_len(n)) {
      idx <- ((i-1L)*dimension+1L):(i*dimension)
      row <- values[idx]
      norm <- sqrt(sum(row*row))
      if (norm<=0) stop("zero embedding row")
      out[i,] <- row/norm
    }
    out
  }
  visual <- normalize(images)
  language <- normalize(texts)
  softplus <- function(x) pmax(x,0)+log1p(exp(-abs(x)))
  total <- 0.0
  for (i in seq_len(n)) {
    for (j in seq_len(n)) {
      similarity <- sum(visual[i,]*language[j,])
      label <- if (i==j) 1.0 else -1.0
      total <- total+softplus(-label*(logit_scale*similarity+bias))
    }
  }
  as.double(total/(n*n))
}
