# Extract four-neighbor foreground boundary pixels from 0/1 masks.
binary_boundary_extraction <- function(pixels,height,width) {
  valid_dim <- function(d) {
    is.numeric(d) && !is.logical(d) && !is.complex(d) &&
      is.null(dim(d)) && length(d)==1L && is.finite(d) &&
      d==floor(d) && d>=1 && d<=64
  }
  if (!valid_dim(height) || !valid_dim(width) ||
      !is.numeric(pixels) || is.logical(pixels) || is.complex(pixels) ||
      !is.null(dim(pixels)) || length(pixels)!=height*width ||
      any(!is.finite(pixels)) || any(!(pixels %in% c(0,1)))) {
    stop("invalid flat binary image or dimensions")
  }
  height <- as.integer(height)
  width <- as.integer(width)
  result <- integer(height*width)
  for (i in seq_along(pixels)) {
    if (pixels[[i]]==0) next
    row <- (i-1L) %/% width+1L
    col <- (i-1L) %% width+1L
    if (row==1L || row==height || col==1L || col==width ||
        pixels[[i-width]]==0 || pixels[[i+width]]==0 ||
        pixels[[i-1L]]==0 || pixels[[i+1L]]==0) result[[i]] <- 1L
  }
  as.integer(result)
}
