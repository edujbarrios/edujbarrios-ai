# Hardest nonmatching image->text and text->image in-batch indices.
image_text_hard_negatives <- function(images, texts, n, dimension) {
  valid_int <- function(value, low, high) {
    is.numeric(value) && !is.complex(value) &&
      is.null(dim(value)) && length(value)==1L &&
      is.finite(value) && value==floor(value) &&
      value>=low && value<=high
  }
  if (!valid_int(n,2,64) || !valid_int(dimension,1,128) ||
      !is.numeric(images) || is.complex(images) ||
      !is.numeric(texts) || is.complex(texts) ||
      !is.null(dim(images)) || !is.null(dim(texts)) ||
      length(images)!=n*dimension || length(texts)!=n*dimension ||
      any(!is.finite(images)) || any(!is.finite(texts)) ||
      any(abs(images)>1000) || any(abs(texts)>1000)) {
    stop("invalid image/text embeddings or dimensions")
  }
  n <- as.integer(n)
  dimension <- as.integer(dimension)
  normalize <- function(values) {
    out <- matrix(0,nrow=n,ncol=dimension)
    for (i in seq_len(n)) {
      idx <- ((i-1L)*dimension+1L):(i*dimension)
      row <- as.double(values[idx])
      norm <- sqrt(sum(row*row))
      if (norm<=0) stop("zero embedding vector")
      out[i,] <- row/norm
    }
    out
  }
  visual <- normalize(images)
  language <- normalize(texts)
  scores <- visual %*% t(language)
  result <- integer(2*n)
  for (i in seq_len(n)) {
    row <- scores[i,]
    col <- scores[,i]
    row[[i]] <- -Inf
    col[[i]] <- -Inf
    result[[i]] <- which.max(row)-1L
    result[[n+i]] <- which.max(col)-1L
  }
  result
}
