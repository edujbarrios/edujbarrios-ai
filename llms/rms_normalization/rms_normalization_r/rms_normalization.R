# Root mean square LLM token normalization with feature-wise gains.
rms_normalization <- function(x, gain, epsilon) {
  if (!is.numeric(x) || is.complex(x) || !is.null(dim(x)) ||
      !is.numeric(gain) || is.complex(gain) || !is.null(dim(gain)) ||
      length(x)<1L || length(x)>1024L || length(gain)!=length(x) ||
      any(!is.finite(x)) || any(!is.finite(gain)) ||
      any(abs(x)>1000) || any(abs(gain)>1000) ||
      !is.numeric(epsilon) || is.complex(epsilon) ||
      !is.null(dim(epsilon)) || length(epsilon)!=1L ||
      !is.finite(epsilon) || epsilon<1e-8 || epsilon>1) {
    stop("invalid features, gains or epsilon")
  }
  inv <- 1/sqrt(sum(x*x)/length(x)+epsilon)
  as.double(x*inv*gain)
}
