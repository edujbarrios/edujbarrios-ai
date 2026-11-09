# Exposed unit-edge perimeter for a four-neighbor binary mask.
binary_perimeter <- function(pixels,height,width) {
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
  total <- 0L
  for (index in seq_along(pixels)) {
    if (pixels[[index]]==0) next
    row <- (index-1L) %/% width+1L
    col <- (index-1L) %% width+1L
    if (row==1L || pixels[[index-width]]==0) total <- total+1L
    if (row==height || pixels[[index+width]]==0) total <- total+1L
    if (col==1L || pixels[[index-1L]]==0) total <- total+1L
    if (col==width || pixels[[index+1L]]==0) total <- total+1L
  }
  as.integer(total)
}
