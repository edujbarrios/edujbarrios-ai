# Half-pixel-center bilinear resize of a flat row-major grayscale image.
bilinear_resize <- function(pixels, height, width, output_height, output_width) {
  valid_dim <- function(d) {
    is.numeric(d) && !is.complex(d) && length(d)==1L &&
      is.null(dim(d)) && is.finite(d) && d==floor(d) && d>=1 && d<=64
  }
  if (!valid_dim(height) || !valid_dim(width) ||
      !valid_dim(output_height) || !valid_dim(output_width) ||
      !is.numeric(pixels) || is.complex(pixels) ||
      !is.null(dim(pixels)) || length(pixels)!=height*width ||
      any(!is.finite(pixels)) || any(abs(pixels)>1000000)) {
    stop("invalid dimensions or pixel data")
  }
  height <- as.integer(height)
  width <- as.integer(width)
  output_height <- as.integer(output_height)
  output_width <- as.integer(output_width)
  coord <- function(index, source_size, dest_size) {
    max(0.0,min(source_size-1.0,(index+0.5)*source_size/dest_size-0.5))
  }
  out <- numeric(output_height*output_width)
  for (r in seq_len(output_height)) {
    y <- coord(r-1L,height,output_height)
    y0 <- floor(y)
    y1 <- min(y0+1L,height-1L)
    wy <- y-y0
    for (c in seq_len(output_width)) {
      x <- coord(c-1L,width,output_width)
      x0 <- floor(x)
      x1 <- min(x0+1L,width-1L)
      wx <- x-x0
      a <- (1-wx)*pixels[[y0*width+x0+1L]] +
        wx*pixels[[y0*width+x1+1L]]
      b <- (1-wx)*pixels[[y1*width+x0+1L]] +
        wx*pixels[[y1*width+x1+1L]]
      out[[(r-1L)*output_width+c]] <- (1-wy)*a+wy*b
    }
  }
  out
}
