# Kendall tau-b from pairwise concordance, discordance and exclusive ties.
kendall_tau_b <- function(x,y) {
  if (!is.numeric(x) || !is.numeric(y) || is.complex(x) || is.complex(y) ||
      !is.null(dim(x)) || !is.null(dim(y)) ||
      length(x)<2L || length(x)>128L || length(x)!=length(y) ||
      any(!is.finite(x)) || any(!is.finite(y)) ||
      any(abs(x)>1000000) || any(abs(y)>1000000)) {
    stop("invalid paired observations")
  }
  concordant <- discordant <- tie_x <- tie_y <- 0.0
  n <- length(x)
  for (i in seq_len(n-1L)) for (j in seq.int(i+1L,n)) {
    sx <- sign(x[[i]]-x[[j]])
    sy <- sign(y[[i]]-y[[j]])
    if (sx==0 && sy==0) next
    if (sx==0) tie_x <- tie_x+1
    else if (sy==0) tie_y <- tie_y+1
    else if (sx==sy) concordant <- concordant+1
    else discordant <- discordant+1
  }
  denominator <- sqrt((concordant+discordant+tie_x)*
                      (concordant+discordant+tie_y))
  if (denominator==0) stop("tau-b undefined for constant ranks")
  max(-1.0,min(1.0,(concordant-discordant)/denominator))
}
