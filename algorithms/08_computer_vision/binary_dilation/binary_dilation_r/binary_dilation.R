# Binary dilation using a 3x3 square and zero outside the image.
binary_dilation <- function(pixels, height, width) {
  valid_dim <- function(d) {
    is.numeric(d) && !is.complex(d) && is.null(dim(d)) &&
      length(d)==1L && is.finite(d) && d==floor(d) &&
      d>=1 && d<=64
  }
  if (!valid_dim(height) || !valid_dim(width) ||
      !is.numeric(pixels) || is.complex(pixels) ||
      !is.null(dim(pixels)) || length(pixels)!=height*width ||
      any(!is.finite(pixels)) ||
      any(pixels!=floor(pixels)) || any(!(pixels %in% c(0,1)))) {
    stop("invalid binary mask or dimensions")
  }
  height <- as.integer(height)
  width <- as.integer(width)
  result <- integer(height*width)
  for (r in seq_len(height)) {
    for (c in seq_len(width)) {
      hit <- 0L
      for (dr in -1:1) for (dc in -1:1) {
        y <- r+dr
        x <- c+dc
        if (y>=1 && y<=height && x>=1 && x<=width &&
            pixels[[(y-1L)*width+x]]==1) hit <- 1L
      }
      result[[(r-1L)*width+c]] <- hit
    }
  }
  result
}
