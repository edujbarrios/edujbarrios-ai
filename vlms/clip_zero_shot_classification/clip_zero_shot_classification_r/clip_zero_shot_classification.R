# CLIP-style zero-shot class probabilities from matched embedding dimensions.
clip_zero_shot_classification <- function(image, texts, classes, dimension,
                                           logit_scale) {
  valid_int <- function(value,low,high) {
    is.numeric(value) && !is.complex(value) && length(value)==1L &&
      is.null(dim(value)) && is.finite(value) &&
      value==floor(value) && value>=low && value<=high
  }
  if (!valid_int(classes,2,64) || !valid_int(dimension,1,128) ||
      !is.numeric(logit_scale) || is.complex(logit_scale) ||
      length(logit_scale)!=1L || !is.finite(logit_scale) ||
      logit_scale<=0 || logit_scale>100 ||
      !is.numeric(image) || is.complex(image) ||
      !is.numeric(texts) || is.complex(texts) ||
      !is.null(dim(image)) || !is.null(dim(texts)) ||
      length(image)!=dimension || length(texts)!=classes*dimension ||
      any(!is.finite(image)) || any(!is.finite(texts)) ||
      any(abs(image)>1000) || any(abs(texts)>1000)) {
    stop("invalid image/text embeddings, dimensions or logit scale")
  }
  classes <- as.integer(classes)
  dimension <- as.integer(dimension)
  image_norm <- sqrt(sum(image*image))
  if (image_norm<=0) stop("zero image embedding")
  scores <- numeric(classes)
  for (i in seq_len(classes)) {
    idx <- ((i-1L)*dimension+1L):(i*dimension)
    row <- texts[idx]
    text_norm <- sqrt(sum(row*row))
    if (text_norm<=0) stop("zero text embedding")
    scores[[i]] <- logit_scale*sum(image*row)/(image_norm*text_norm)
  }
  weights <- exp(scores-max(scores))
  sum_weights <- sum(weights)
  if (!is.finite(sum_weights) || sum_weights<=0)
    stop("nonfinite normalization")
  as.double(weights/sum_weights)
}
