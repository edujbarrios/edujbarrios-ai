# Binary morphological opening with 3x3 square and zero padding.
binary_opening <- function(image, width, height) {
  valid_dimension <- function(v) is.numeric(v) && !is.complex(v) &&
    length(v)==1L && is.finite(v) && v==floor(v) && v>=1 && v<=64
  if (!valid_dimension(width) || !valid_dimension(height) ||
      !is.numeric(image) || is.complex(image) ||
      length(image)!=width*height || any(!is.finite(image)) ||
      any(image!=0 & image!=1)) stop("invalid binary image or dimensions")
  width <- as.integer(width)
  height <- as.integer(height)
  values <- as.integer(image)
  eroded <- integer(length(values))
  for (y in seq_len(height)) for (x in seq_len(width)) {
    keep <- 1L
    for (dy in -1:1) for (dx in -1:1) {
      ny <- y+dy; nx <- x+dx
      if (ny<1L || nx<1L || ny>height || nx>width ||
          (ny>=1L && nx>=1L && ny<=height && nx<=width &&
           values[(ny-1L)*width+nx]==0L)) keep <- 0L
    }
    eroded[(y-1L)*width+x] <- keep
  }
  output <- integer(length(values))
  for (y in seq_len(height)) for (x in seq_len(width)) {
    keep <- 0L
    for (dy in -1:1) for (dx in -1:1) {
      ny <- y+dy; nx <- x+dx
      if (ny>=1L && nx>=1L && ny<=height && nx<=width &&
          eroded[(ny-1L)*width+nx]==1L) keep <- 1L
    }
    output[(y-1L)*width+x] <- keep
  }
  output
}
