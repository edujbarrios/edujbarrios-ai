# Image-to-text/text-to-image matched-pair recall at K, diagonal positives.
image_text_recall_at_k <- function(images, texts, n, dimension, k) {
  valid_int <- function(value, low, high) {
    is.numeric(value) && !is.complex(value) && length(value)==1L &&
      is.null(dim(value)) && is.finite(value) &&
      value==floor(value) && value>=low && value<=high
  }
  if (!valid_int(n,2,64) || !valid_int(dimension,1,128) ||
      !valid_int(k,1,n) || !is.numeric(images) || is.complex(images) ||
      !is.numeric(texts) || is.complex(texts) ||
      !is.null(dim(images)) || !is.null(dim(texts)) ||
      length(images)!=n*dimension || length(texts)!=n*dimension ||
      any(!is.finite(images)) || any(!is.finite(texts)) ||
      any(abs(images)>1000) || any(abs(texts)>1000)) {
    stop("invalid shapes, K, or embedding values")
  }
  n <- as.integer(n)
  dimension <- as.integer(dimension)
  normalize <- function(values) {
    out <- matrix(0,nrow=n,ncol=dimension)
    for (i in seq_len(n)) {
      idx <- ((i-1L)*dimension+1L):(i*dimension)
      row <- values[idx]
      norm <- sqrt(sum(row*row))
      if (norm<=0) stop("zero embedding vector")
      out[i,] <- row/norm
    }
    out
  }
  visual <- normalize(images)
  language <- normalize(texts)
  similarities <- visual %*% t(language)
  image_hits <- 0
  text_hits <- 0
  for (i in seq_len(n)) {
    correct <- similarities[i,i]
    row_rank <- sum(similarities[i,]>correct |
      (similarities[i,]==correct & seq_len(n)<i))
    col_rank <- sum(similarities[,i]>correct |
      (similarities[,i]==correct & seq_len(n)<i))
    image_hits <- image_hits + as.integer(row_rank<k)
    text_hits <- text_hits + as.integer(col_rank<k)
  }
  as.double(c(image_hits/n,text_hits/n))
}
