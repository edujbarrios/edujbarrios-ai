# Deterministic four-connected component labeling of flat binary masks.
binary_connected_components <- function(pixels, height, width) {
  valid_dim <- function(d) {
    is.numeric(d) && !is.complex(d) && is.null(dim(d)) &&
      length(d) == 1L && is.finite(d) && d == floor(d) &&
      d >= 1 && d <= 64
  }
  if (!valid_dim(height) || !valid_dim(width) ||
      !is.numeric(pixels) || is.complex(pixels) || !is.null(dim(pixels)) ||
      length(pixels) != height * width || any(!is.finite(pixels)) ||
      any(!(pixels %in% c(0, 1)))) {
    stop("invalid binary mask or dimensions")
  }
  height <- as.integer(height)
  width <- as.integer(width)
  n <- height * width
  labels <- integer(n)
  queue <- integer(n)
  count <- 0L
  for (start in seq_len(n)) {
    if (pixels[[start]] == 0 || labels[[start]] != 0L) next
    count <- count + 1L
    labels[[start]] <- count
    head <- 1L
    tail <- 1L
    queue[[tail]] <- start
    while (head <= tail) {
      index <- queue[[head]]
      head <- head + 1L
      row <- (index - 1L) %/% width + 1L
      col <- (index - 1L) %% width + 1L
      neighbors <- c(
        if (row > 1L) index - width else integer(0),
        if (row < height) index + width else integer(0),
        if (col > 1L) index - 1L else integer(0),
        if (col < width) index + 1L else integer(0)
      )
      for (next_index in neighbors) {
        if (pixels[[next_index]] == 1 && labels[[next_index]] == 0L) {
          labels[[next_index]] <- count
          tail <- tail + 1L
          queue[[tail]] <- next_index
        }
      }
    }
  }
  list(labels=labels, count=count)
}
