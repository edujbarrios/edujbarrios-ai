# Deterministic 8-bit histogram equalization with a fixed global CDF.
grayscale_histogram_equalization <- function(pixels, height, width) {
  valid_dim <- function(d) {
    is.numeric(d) && !is.logical(d) && !is.complex(d) &&
      is.null(dim(d)) && length(d)==1L && is.finite(d) &&
      d==floor(d) && d>=1 && d<=64
  }
  if (!valid_dim(height) || !valid_dim(width) ||
      !is.numeric(pixels) || is.logical(pixels) || is.complex(pixels) ||
      !is.null(dim(pixels)) || length(pixels)!=height*width ||
      any(!is.finite(pixels)) || any(pixels!=floor(pixels)) ||
      any(pixels<0 | pixels>255)) stop("invalid grayscale image or dimensions")
  pixels <- as.integer(pixels)
  counts <- tabulate(pixels+1L, nbins=256L)
  cdf_min <- counts[which(counts>0L)[[1L]]]
  n <- length(pixels)
  if (cdf_min==n) return(as.integer(pixels))
  denominator <- n-cdf_min
  cumulative <- cumsum(counts)
  mapped <- integer(256L)
  for (i in seq_len(256L)) {
    if (cumulative[[i]]>cdf_min) {
      numerator <- 255L*(cumulative[[i]]-cdf_min)
      mapped[[i]] <- as.integer((2L*numerator+denominator) %/% (2L*denominator))
    }
  }
  as.integer(mapped[pixels+1L])
}
