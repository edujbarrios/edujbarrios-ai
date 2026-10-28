# One text-query attention over image-patch keys and value vectors.
cross_modal_attention <- function(query, keys, values, patches, dimension) {
  valid_dim <- function(x) {
    is.numeric(x) && !is.complex(x) && length(x)==1L &&
      is.null(dim(x)) && is.finite(x) && x==floor(x) &&
      x>=1 && x<=64
  }
  valid_values <- function(x, n) {
    is.numeric(x) && !is.complex(x) && is.null(dim(x)) &&
      length(x)==n && all(is.finite(x)) && all(abs(x)<=1000)
  }
  if (!valid_dim(patches) || !valid_dim(dimension) ||
      !valid_values(query,dimension) ||
      !valid_values(keys,patches*dimension) ||
      !valid_values(values,patches*dimension)) {
    stop("invalid dimensions or embeddings")
  }
  patches <- as.integer(patches)
  dimension <- as.integer(dimension)
  scores <- numeric(patches)
  for (p in seq_len(patches)) {
    ids <- ((p-1)*dimension+1):(p*dimension)
    scores[[p]] <- sum(query*keys[ids])/sqrt(dimension)
  }
  exponents <- exp(scores-max(scores))
  weights <- as.double(exponents/sum(exponents))
  context <- numeric(dimension)
  for (p in seq_len(patches)) {
    ids <- ((p-1)*dimension+1):(p*dimension)
    context <- context+weights[[p]]*values[ids]
  }
  list(weights=weights,context=as.double(context))
}
