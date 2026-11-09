# Horizontal and vertical binary projection profiles.
binary_projection_profiles <- function(pixels,height,width) {
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
  result <- integer(height+width)
  for (index in seq_along(pixels)) {
    if (pixels[[index]]!=0) {
      row <- (index-1L) %/% width+1L
      col <- (index-1L) %% width+1L
      result[[row]] <- result[[row]]+1L
      result[[height+col]] <- result[[height+col]]+1L
    }
  }
  as.integer(result)
}
