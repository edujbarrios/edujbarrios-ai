# Otsu's global 8-bit grayscale threshold (smallest maximizing t).
otsu_threshold <- function(pixels) {
  if (!is.numeric(pixels) || is.complex(pixels) ||
      !is.null(dim(pixels)) || length(pixels)<1L || length(pixels)>4096L ||
      any(!is.finite(pixels)) || any(pixels!=floor(pixels)) ||
      any(pixels<0) || any(pixels>255)) {
    stop("invalid 8-bit grayscale pixels")
  }
  histogram <- tabulate(as.integer(pixels)+1L,nbins=256L)
  n <- length(pixels)
  sum_all <- sum(pixels)
  count_low <- 0
  sum_low <- 0
  best <- -1.0
  result <- 0L
  for (t in 0:254) {
    count_low <- count_low+histogram[[t+1L]]
    sum_low <- sum_low+t*histogram[[t+1L]]
    if (count_low==0 || count_low==n) next
    difference <- sum_all*count_low-n*sum_low
    score <- difference*difference/(count_low*(n-count_low))
    if (score>best) {
      best <- score
      result <- as.integer(t)
    }
  }
  result
}
