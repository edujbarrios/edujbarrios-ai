# Inclusive row-major integral image for unsigned 8-bit grayscale pixels.
integral_image <- function(pixels, height, width) {
  valid_dim <- function(d) {
    is.numeric(d) && !is.logical(d) && !is.complex(d) &&
      is.null(dim(d)) && length(d) == 1L && is.finite(d) &&
      d == floor(d) && d >= 1 && d <= 64
  }
  if (!valid_dim(height) || !valid_dim(width) ||
      !is.numeric(pixels) || is.logical(pixels) || is.complex(pixels) ||
      !is.null(dim(pixels)) || length(pixels) != height * width ||
      any(!is.finite(pixels)) || any(pixels != floor(pixels)) ||
      any(pixels < 0 | pixels > 255)) {
    stop("invalid grayscale image or dimensions")
  }
  height <- as.integer(height)
  width <- as.integer(width)
  output <- integer(height * width)
  for (row in seq_len(height)) {
    row_sum <- 0L
    for (col in seq_len(width)) {
      i <- (row - 1L) * width + col
      row_sum <- row_sum + as.integer(pixels[[i]])
      output[[i]] <- row_sum + if (row > 1L) output[[i - width]] else 0L
    }
  }
  output
}
