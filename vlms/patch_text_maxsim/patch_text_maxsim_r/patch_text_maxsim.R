# Late-interaction text-token to visual-patch maximum cosine similarity.
patch_text_maxsim <- function(patches, n_patches, tokens, n_tokens, dimension) {
  valid_dim <- function(x, limit) {
    is.numeric(x) && !is.complex(x) && is.null(dim(x)) &&
      length(x)==1L && is.finite(x) && x==floor(x) &&
      x>=1 && x<=limit
  }
  valid_vec <- function(x, count) {
    is.numeric(x) && !is.complex(x) && is.null(dim(x)) &&
      length(x)==count && all(is.finite(x)) && all(abs(x)<=1000)
  }
  if (!valid_dim(n_patches,32) || !valid_dim(n_tokens,32) ||
      !valid_dim(dimension,64) ||
      !valid_vec(patches,n_patches*dimension) ||
      !valid_vec(tokens,n_tokens*dimension)) {
    stop("invalid embedding shapes or values")
  }
  n_patches <- as.integer(n_patches)
  n_tokens <- as.integer(n_tokens)
  dimension <- as.integer(dimension)
  normalize <- function(flat, count) {
    out <- matrix(0,nrow=count,ncol=dimension)
    for (i in seq_len(count)) {
      ids <- ((i-1L)*dimension+1L):(i*dimension)
      row <- flat[ids]
      norm <- sqrt(sum(row*row))
      if (norm<=0) stop("zero embedding")
      out[i,] <- row/norm
    }
    out
  }
  visual <- normalize(patches,n_patches)
  language <- normalize(tokens,n_tokens)
  matches <- integer(n_tokens)
  total <- 0.0
  for (t in seq_len(n_tokens)) {
    similarities <- as.vector(visual %*% language[t,])
    best <- which.max(similarities)
    matches[[t]] <- as.integer(best-1L)
    total <- total+similarities[[best]]
  }
  list(matches=matches,score=as.double(total/n_tokens))
}
