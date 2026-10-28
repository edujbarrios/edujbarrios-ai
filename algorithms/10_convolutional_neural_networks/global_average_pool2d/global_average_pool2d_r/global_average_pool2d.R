# Global average pooling over spatial dimensions for CHW tensors.
global_average_pool2d <- function(values, channels, height, width) {
  valid_dim <- function(x, maxval) {
    is.numeric(x) && !is.complex(x) && is.null(dim(x)) &&
      length(x)==1L && is.finite(x) && x==floor(x) &&
      x>=1 && x<=maxval
  }
  if (!valid_dim(channels,16) || !valid_dim(height,32) ||
      !valid_dim(width,32) || !is.numeric(values) ||
      is.complex(values) || !is.null(dim(values)) ||
      length(values)!=channels*height*width ||
      any(!is.finite(values)) || any(abs(values)>1000)) {
    stop("invalid CHW tensor")
  }
  channels <- as.integer(channels)
  spatial <- as.integer(height*width)
  out <- numeric(channels)
  for (c in seq_len(channels)) {
    ids <- ((c-1L)*spatial+1L):(c*spatial)
    out[[c]] <- sum(values[ids])/spatial
  }
  out
}
