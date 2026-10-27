# Symmetric contrastive CE loss on paired row-major embeddings.
clip_contrastive_loss <- function(images, texts, n, dimension, logit_scale) {
  scalar_int <- function(x, lower, upper) {
    is.numeric(x) && !is.complex(x) && is.null(dim(x)) &&
      length(x)==1L && is.finite(x) && x==floor(x) &&
      x>=lower && x<=upper
  }
  if (!scalar_int(n,2,64) || !scalar_int(dimension,1,128) ||
      !is.numeric(logit_scale) || is.complex(logit_scale) ||
      length(logit_scale)!=1L || !is.finite(logit_scale) ||
      logit_scale<=0 || logit_scale>100 ||
      !is.numeric(images) || !is.numeric(texts) ||
      is.complex(images) || is.complex(texts) ||
      !is.null(dim(images)) || !is.null(dim(texts)) ||
      length(images)!=n*dimension || length(texts)!=n*dimension ||
      any(!is.finite(images)) || any(!is.finite(texts)) ||
      any(abs(images)>1000) || any(abs(texts)>1000)) {
    stop("invalid image/text embeddings, dimensions or logit scale")
  }
  n <- as.integer(n)
  dimension <- as.integer(dimension)
  normalize <- function(values) {
    normalized <- matrix(0,nrow=n,ncol=dimension)
    for (i in seq_len(n)) {
      indices <- ((i-1L)*dimension+1L):(i*dimension)
      row <- as.double(values[indices])
      magnitude <- sqrt(sum(row*row))
      if (magnitude<=0) stop("zero embedding vector")
      normalized[i,] <- row/magnitude
    }
    normalized
  }
  visual <- normalize(images)
  language <- normalize(texts)
  logits <- logit_scale*(visual %*% t(language))
  logsumexp <- function(v) {
    maximum <- max(v)
    maximum + log(sum(exp(v-maximum)))
  }
  total <- 0.0
  for (i in seq_len(n)) {
    total <- total + logsumexp(logits[i,]) - logits[i,i] +
      logsumexp(logits[,i]) - logits[i,i]
  }
  result <- total/(2*n)
  if (!is.finite(result)) stop("nonfinite loss")
  max(0.0,as.double(result))
}
