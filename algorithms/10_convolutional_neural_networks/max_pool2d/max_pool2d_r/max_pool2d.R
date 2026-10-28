# Non-overlapping 2x2 max pooling, stride 2, no padding.
max_pool2d <- function(values, height, width) {
  valid_dim <- function(v) {
    is.numeric(v) && !is.complex(v) && is.null(dim(v)) &&
      length(v)==1L && is.finite(v) &&
      v==floor(v) && v>=2 && v<=32
  }
  if (!valid_dim(height) || !valid_dim(width) ||
      !is.numeric(values) || is.complex(values) ||
      !is.null(dim(values)) || length(values)!=height*width ||
      any(!is.finite(values)) || any(abs(values)>1000)) {
    stop("invalid input or dimensions")
  }
  oh <- as.integer(floor(height/2))
  ow <- as.integer(floor(width/2))
  out <- numeric(oh*ow)
  for (r in seq_len(oh)) for (c in seq_len(ow)) {
    at <- (2*r-2)*width + 2*c-1
    out[[(r-1)*ow+c]] <- max(values[c(at,at+1,at+width,at+width+1)])
  }
  out
}
