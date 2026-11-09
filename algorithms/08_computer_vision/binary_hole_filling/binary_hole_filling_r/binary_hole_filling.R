# Four-neighbor binary hole filling from all border-connected zeros.
binary_hole_filling <- function(pixels,height,width) {
  valid_dim <- function(d) {
    is.numeric(d) && !is.logical(d) && !is.complex(d) &&
      is.null(dim(d)) && length(d)==1L && is.finite(d) &&
      d==floor(d) && d>=1 && d<=64
  }
  if (!valid_dim(height) || !valid_dim(width) ||
      !is.numeric(pixels) || is.logical(pixels) || is.complex(pixels) ||
      !is.null(dim(pixels)) || length(pixels)!=height*width ||
      any(!is.finite(pixels)) || any(!(pixels %in% c(0,1)))) {
    stop("invalid binary mask or dimensions")
  }
  height <- as.integer(height)
  width <- as.integer(width)
  n <- height*width
  seen <- rep.int(FALSE,n)
  queue <- integer(n)
  head <- 1L
  tail <- 0L
  for (index in seq_len(n)) {
    row <- (index-1L) %/% width+1L
    col <- (index-1L) %% width+1L
    if (pixels[[index]]==0 &&
        (row==1L || row==height || col==1L || col==width)) {
      seen[[index]] <- TRUE
      tail <- tail+1L
      queue[[tail]] <- index
    }
  }
  while (head<=tail) {
    index <- queue[[head]]
    head <- head+1L
    row <- (index-1L) %/% width+1L
    col <- (index-1L) %% width+1L
    neighbors <- c(
      if (row>1L) index-width else integer(0),
      if (row<height) index+width else integer(0),
      if (col>1L) index-1L else integer(0),
      if (col<width) index+1L else integer(0)
    )
    for (next_index in neighbors) {
      if (pixels[[next_index]]==0 && !seen[[next_index]]) {
        seen[[next_index]] <- TRUE
        tail <- tail+1L
        queue[[tail]] <- next_index
      }
    }
  }
  as.integer(!seen)
}
