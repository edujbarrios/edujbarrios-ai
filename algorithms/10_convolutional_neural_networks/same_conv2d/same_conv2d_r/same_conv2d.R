# Single-channel same-sized zero-padded 2D cross-correlation.
same_conv2d <- function(image,height,width,kernel,kh,kw) {
  valid_dim <- function(x,min_value,max_value) {
    is.numeric(x) && !is.logical(x) && !is.complex(x) && is.null(dim(x)) &&
      length(x)==1L && is.finite(x) && x==floor(x) &&
      x>=min_value && x<=max_value
  }
  if (!valid_dim(height,1,32) || !valid_dim(width,1,32) ||
      !valid_dim(kh,1,5) || !valid_dim(kw,1,5) ||
      kh%%2==0 || kw%%2==0 || !is.numeric(image) || is.logical(image) ||
      is.complex(image) || !is.null(dim(image)) ||
      !is.numeric(kernel) || is.logical(kernel) || is.complex(kernel) ||
      !is.null(dim(kernel)) || length(image)!=height*width ||
      length(kernel)!=kh*kw || any(!is.finite(image)) ||
      any(!is.finite(kernel)) || any(abs(image)>1000) || any(abs(kernel)>1000)) {
    stop("invalid flat image, kernel or dimensions")
  }
  height <- as.integer(height);width <- as.integer(width)
  kh <- as.integer(kh);kw <- as.integer(kw)
  result <- numeric(height*width)
  for(row in seq_len(height)) {
    for(col in seq_len(width)) {
      value <- 0.0
      for(ky in seq_len(kh)) {
        rr <- row+ky-((kh+1L)%/%2L)
        if(rr<1L || rr>height)next
        for(kx in seq_len(kw)) {
          cc <- col+kx-((kw+1L)%/%2L)
          if(cc>=1L && cc<=width) {
            value <- value + image[[(rr-1L)*width+cc]]*kernel[[(ky-1L)*kw+kx]]
          }
        }
      }
      result[[(row-1L)*width+col]] <- value
    }
  }
  as.double(result)
}
