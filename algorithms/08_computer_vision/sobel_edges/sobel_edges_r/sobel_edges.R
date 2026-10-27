# Sobel 3x3 grayscale gradient magnitude, replicate-edge padding.
sobel_edges <- function(pixels, height, width) {
  valid_dim <- function(d) {
    is.numeric(d) && !is.complex(d) && length(d)==1L &&
      is.null(dim(d)) && is.finite(d) && d==floor(d) &&
      d>=1 && d<=64
  }
  if (!valid_dim(height) || !valid_dim(width) ||
      !is.numeric(pixels) || is.complex(pixels) ||
      !is.null(dim(pixels)) || length(pixels)!=height*width ||
      any(!is.finite(pixels)) || any(abs(pixels)>1000)) {
    stop("invalid dimensions or image pixels")
  }
  height <- as.integer(height)
  width <- as.integer(width)
  kx <- matrix(c(-1,0,1,-2,0,2,-1,0,1),nrow=3,byrow=TRUE)
  ky <- matrix(c(-1,-2,-1,0,0,0,1,2,1),nrow=3,byrow=TRUE)
  result <- numeric(height*width)
  for (row in seq_len(height)) {
    for (col in seq_len(width)) {
      gx <- 0.0
      gy <- 0.0
      for (y in seq_len(3)) for (x in seq_len(3)) {
        r <- max(1L,min(height,row+y-2L))
        c <- max(1L,min(width,col+x-2L))
        pixel <- pixels[[(r-1L)*width+c]]
        gx <- gx + kx[y,x]*pixel
        gy <- gy + ky[y,x]*pixel
      }
      result[[(row-1L)*width+col]] <- sqrt(gx*gx+gy*gy)
    }
  }
  result
}
