# Valid (unpadded) single-channel CNN cross-correlation.
valid_conv2d <- function(image, height, width, kernel, kh, kw) {
  valid_dim <- function(x, cap) {
    is.numeric(x) && !is.complex(x) && length(x)==1L &&
      is.null(dim(x)) && is.finite(x) &&
      x==floor(x) && x>=1 && x<=cap
  }
  if (!valid_dim(height,32) || !valid_dim(width,32) ||
      !valid_dim(kh,8) || !valid_dim(kw,8) ||
      kh>height || kw>width ||
      !is.numeric(image) || is.complex(image) || !is.null(dim(image)) ||
      !is.numeric(kernel) || is.complex(kernel) || !is.null(dim(kernel)) ||
      length(image)!=height*width || length(kernel)!=kh*kw ||
      any(!is.finite(image)) || any(!is.finite(kernel)) ||
      any(abs(image)>1000) || any(abs(kernel)>1000)) {
    stop("invalid input or kernel")
  }
  oh <- as.integer(height-kh+1)
  ow <- as.integer(width-kw+1)
  out <- numeric(oh*ow)
  for (r in seq_len(oh)) for (c in seq_len(ow)) {
    v <- 0.0
    for (y in seq_len(kh)) for (x in seq_len(kw)) {
      v <- v + image[[(r+y-2)*width+c+x-1]]*kernel[[(y-1)*kw+x]]
    }
    out[[(r-1)*ow+c]] <- v
  }
  out
}
