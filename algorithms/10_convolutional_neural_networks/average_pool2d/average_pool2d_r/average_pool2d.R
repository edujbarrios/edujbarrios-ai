# Nonoverlapping 2x2 stride-2 mean pooling, dropping incomplete borders.
average_pool2d <- function(image,height,width) {
  valid_dim <- function(d) {
    is.numeric(d) && !is.logical(d) && !is.complex(d) &&
      is.null(dim(d)) && length(d)==1L && is.finite(d) &&
      d==floor(d) && d>=2 && d<=32
  }
  if (!valid_dim(height) || !valid_dim(width) ||
      !is.numeric(image) || is.logical(image) || is.complex(image) ||
      !is.null(dim(image)) || length(image)!=height*width ||
      any(!is.finite(image)) || any(abs(image)>1000)) {
    stop("invalid image values or shape")
  }
  height <- as.integer(height);width <- as.integer(width)
  oh <- height%/%2L; ow <- width%/%2L
  result <- numeric(oh*ow)
  for(r in seq_len(oh))for(c in seq_len(ow)) {
    i <- (2L*r-2L)*width+2L*c-1L
    result[[(r-1L)*ow+c]] <- (
      image[[i]]+image[[i+1L]]+image[[i+width]]+image[[i+width+1L]]
    )/4.0
  }
  as.double(result)
}
