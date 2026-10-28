# Depthwise CHW cross-correlation with same zero padding.
depthwise_conv2d <- function(values, channels, height, width, kernels, kh, kw) {
  dim_ok <- function(z, mx) {
    is.numeric(z) && !is.complex(z) && is.null(dim(z)) &&
      length(z)==1L && is.finite(z) && z==floor(z) && z>=1 && z<=mx
  }
  value_ok <- function(z, len) {
    is.numeric(z) && !is.complex(z) && is.null(dim(z)) &&
      length(z)==len && all(is.finite(z)) && all(abs(z)<=1000)
  }
  if (!dim_ok(channels,8) || !dim_ok(height,16) || !dim_ok(width,16) ||
      !dim_ok(kh,5) || !dim_ok(kw,5) || kh%%2!=1 || kw%%2!=1 ||
      !value_ok(values,channels*height*width) ||
      !value_ok(kernels,channels*kh*kw)) stop("invalid depthwise CHW tensor")
  channels <- as.integer(channels)
  height <- as.integer(height)
  width <- as.integer(width)
  kh <- as.integer(kh)
  kw <- as.integer(kw)
  out <- numeric(channels*height*width)
  for (c in seq_len(channels)) for (r in seq_len(height)) for (s in seq_len(width)) {
    total <- 0.0
    for (i in seq_len(kh)) for (j in seq_len(kw)) {
      y <- r+i-1L-kh%/%2L
      x <- s+j-1L-kw%/%2L
      if (y>=1L && y<=height && x>=1L && x<=width) {
        total <- total+values[[(c-1L)*height*width+(y-1L)*width+x]]*
                       kernels[[(c-1L)*kh*kw+(i-1L)*kw+j]]
      }
    }
    out[[(c-1L)*height*width+(r-1L)*width+s]] <- total
  }
  out
}
