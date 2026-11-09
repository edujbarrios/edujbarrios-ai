# Root mean square using a max-absolute scale before squaring.
root_mean_square <- function(values) {
  if (!is.numeric(values) || is.logical(values) || is.complex(values) ||
      !is.null(dim(values)) || length(values)<1L || length(values)>512L ||
      any(!is.finite(values))) {
    stop("expected 1..512 finite real observations")
  }
  x <- as.double(values)
  scale <- max(abs(x))
  if (scale==0) return(0.0)
  squares <- 0.0
  for (v in x) {
    normalized <- v / scale
    squares <- squares + normalized*normalized
  }
  result <- scale*sqrt(squares/length(x))
  if (!is.finite(result)) stop("nonfinite RMS")
  as.double(result)
}
