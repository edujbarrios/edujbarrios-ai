# Binary 3x3 dilation followed by 3x3 erosion with a zero border.
binary_closing <- function(image, width, height) {
  valid_dim <- function(v) is.numeric(v) && !is.complex(v) &&
    length(v) == 1L && is.finite(v) && v == floor(v) && v >= 1 && v <= 64
  if (!valid_dim(width) || !valid_dim(height) || !is.numeric(image) ||
      is.complex(image) || length(image) != width*height ||
      any(!is.finite(image)) || any(image != 0 & image != 1)) {
    stop("invalid binary image or dimensions")
  }
  width <- as.integer(width)
  height <- as.integer(height)
  values <- as.integer(image)
  dilated <- integer(length(values))
  for (y in seq_len(height)) for (x in seq_len(width)) {
    on <- 0L
    for (dy in -1:1) for (dx in -1:1) {
      yy <- y + dy; xx <- x + dx
      if (yy >= 1L && xx >= 1L && yy <= height && xx <= width &&
          values[(yy-1L)*width + xx] == 1L) on <- 1L
    }
    dilated[(y-1L)*width + x] <- on
  }
  result <- integer(length(values))
  for (y in seq_len(height)) for (x in seq_len(width)) {
    on <- 1L
    for (dy in -1:1) for (dx in -1:1) {
      yy <- y + dy; xx <- x + dx
      if (yy < 1L || xx < 1L || yy > height || xx > width ||
          (yy >= 1L && xx >= 1L && yy <= height && xx <= width &&
           dilated[(yy-1L)*width + xx] == 0L)) on <- 0L
    }
    result[(y-1L)*width + x] <- on
  }
  result
}
