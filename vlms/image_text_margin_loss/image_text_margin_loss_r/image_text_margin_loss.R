# Bidirectional hardest-negative hinge loss for matched image/text rows.
image_text_margin_loss <- function(images, texts, n, dimension, margin) {
  valid_int <- function(d,low,high) {
    is.numeric(d) && !is.complex(d) && is.null(dim(d)) &&
      length(d)==1L && is.finite(d) && d==floor(d) &&
      d>=low && d<=high
  }
  if (!valid_int(n,2,64) || !valid_int(dimension,1,128) ||
      !is.numeric(margin) || is.complex(margin) ||
      !is.null(dim(margin)) || length(margin)!=1L ||
      !is.finite(margin) || margin<0 || margin>2 ||
      !is.numeric(images) || is.complex(images) ||
      !is.numeric(texts) || is.complex(texts) ||
      !is.null(dim(images)) || !is.null(dim(texts)) ||
      length(images)!=n*dimension || length(texts)!=n*dimension ||
      any(!is.finite(images)) || any(!is.finite(texts)) ||
      any(abs(images)>1000) || any(abs(texts)>1000)) {
    stop("invalid dimensions, margin, or embedding data")
  }
  n <- as.integer(n)
  dimension <- as.integer(dimension)
  normalize <- function(flat) {
    rows <- matrix(0,nrow=n,ncol=dimension)
    for (i in seq_len(n)) {
      ids <- ((i-1L)*dimension+1L):(i*dimension)
      row <- flat[ids]
      norm <- sqrt(sum(row*row))
      if (norm<=0) stop("zero embedding row")
      rows[i,] <- row/norm
    }
    rows
  }
  visual <- normalize(images)
  language <- normalize(texts)
  score <- visual %*% t(language)
  total <- 0.0
  for (i in seq_len(n)) {
    match <- score[i,i]
    row <- score[i,]
    col <- score[,i]
    row[[i]] <- -Inf
    col[[i]] <- -Inf
    total <- total+max(0,margin-match+max(row))
    total <- total+max(0,margin-match+max(col))
  }
  as.double(total/(2*n))
}
