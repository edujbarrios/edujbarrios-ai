# Global 8-bit min-max contrast stretching, half-up integer rounding.
grayscale_contrast_stretching <- function(pixels, height, width) {
  valid_dim <- function(d) {
    is.numeric(d) && !is.logical(d) && !is.complex(d) &&
      is.null(dim(d)) && length(d)==1L && is.finite(d) &&
      d==floor(d) && d>=1 && d<=64
  }
  if (!valid_dim(height) || !valid_dim(width) ||
      !is.numeric(pixels) || is.logical(pixels) || is.complex(pixels) ||
      !is.null(dim(pixels)) || length(pixels)!=height*width ||
      any(!is.finite(pixels)) || any(pixels!=floor(pixels)) ||
      any(pixels<0 | pixels>255)) {
    stop("invalid grayscale image or dimensions")
  }
  x <- as.integer(pixels)
  lo <- min(x)
  hi <- max(x)
  span <- hi-lo
  if (span==0L) return(as.integer(x))
  as.integer((510L*(x-lo)+span) %/% (2L*span))
}
