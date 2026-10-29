# Fixed 3x3 median filter on flat row-major grayscale images.
median_filter <- function(pixels, height, width) {
  valid_dim <- function(x) {
    is.numeric(x) && !is.complex(x) && length(x) == 1L &&
      is.null(dim(x)) && is.finite(x) && x == floor(x) &&
      x >= 1 && x <= 64
  }
  if (!valid_dim(height) || !valid_dim(width) ||
      !is.numeric(pixels) || is.complex(pixels) ||
      !is.null(dim(pixels)) || length(pixels) != height * width ||
      any(!is.finite(pixels)) || any(abs(pixels) > 1000000)) {
    stop("invalid image dimensions or pixel values")
  }
  height <- as.integer(height)
  width <- as.integer(width)
  out <- numeric(height * width)
  for (row in seq_len(height)) {
    for (col in seq_len(width)) {
      window <- numeric(9L)
      index <- 1L
      for (dr in -1:1) {
        for (dc in -1:1) {
          r <- max(1L, min(height, row + dr))
          c <- max(1L, min(width, col + dc))
          window[[index]] <- pixels[[(r - 1L) * width + c]]
          index <- index + 1L
        }
      }
      out[[(row - 1L) * width + col]] <- sort(window)[[5L]]
    }
  }
  out
}
