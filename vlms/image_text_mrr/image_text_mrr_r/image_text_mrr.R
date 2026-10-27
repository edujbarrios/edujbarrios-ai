# Bidirectional diagonal-positive cosine retrieval mean reciprocal rank.
image_text_mrr <- function(images, texts, n, dimension) {
  valid_dim <- function(value, low, high) {
    is.numeric(value) && !is.complex(value) && length(value)==1L &&
      is.null(dim(value)) && is.finite(value) &&
      value==floor(value) && value>=low && value<=high
  }
  if (!valid_dim(n,2,64) || !valid_dim(dimension,1,128) ||
      !is.numeric(images) || is.complex(images) || !is.null(dim(images)) ||
      !is.numeric(texts) || is.complex(texts) || !is.null(dim(texts)) ||
      length(images)!=n*dimension || length(texts)!=n*dimension ||
      any(!is.finite(images)) || any(!is.finite(texts)) ||
      any(abs(images)>1000) || any(abs(texts)>1000)) {
    stop("invalid dimensions or embeddings")
  }
  n <- as.integer(n)
  dimension <- as.integer(dimension)
  normalize <- function(flat) {
    out <- matrix(0,nrow=n,ncol=dimension)
    for (i in seq_len(n)) {
      ids <- ((i-1L)*dimension+1L):(i*dimension)
      row <- flat[ids]
      norm <- sqrt(sum(row*row))
      if (norm<=0) stop("zero embedding vector")
      out[i,] <- row/norm
    }
    out
  }
  scores <- normalize(images) %*% t(normalize(texts))
  image_total <- 0.0
  text_total <- 0.0
  for (i in seq_len(n)) {
    correct <- scores[i,i]
    row_rank <- 1+sum(scores[i,]>correct |
      (scores[i,]==correct & seq_len(n)<i))
    col_rank <- 1+sum(scores[,i]>correct |
      (scores[,i]==correct & seq_len(n)<i))
    image_total <- image_total+1/row_rank
    text_total <- text_total+1/col_rank
  }
  as.double(c(image_total/n,text_total/n))
}
