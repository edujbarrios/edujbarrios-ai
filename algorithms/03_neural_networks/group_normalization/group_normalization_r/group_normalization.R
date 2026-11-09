# Group normalization for flat channel-major activations.
group_normalization <- function(x,channels,spatial_size,groups,gamma,beta,epsilon) {
  valid_integer <- function(v,lo,hi) {
    is.numeric(v) && !is.logical(v) && !is.complex(v) &&
      is.null(dim(v)) && length(v)==1L && is.finite(v) &&
      v==floor(v) && v>=lo && v<=hi
  }
  if (!valid_integer(channels,1,32) ||
      !valid_integer(spatial_size,1,64) ||
      !valid_integer(groups,1,channels) || channels%%groups!=0) {
    stop("invalid channel, spatial or group dimensions")
  }
  channels <- as.integer(channels)
  spatial_size <- as.integer(spatial_size)
  groups <- as.integer(groups)
  valid_values <- function(v,n) {
    is.numeric(v) && !is.logical(v) && !is.complex(v) &&
      is.null(dim(v)) && length(v)==n && all(is.finite(v)) &&
      all(abs(v)<=1000)
  }
  if (!valid_values(x,channels*spatial_size) ||
      !valid_values(gamma,channels) || !valid_values(beta,channels) ||
      !is.numeric(epsilon) || is.logical(epsilon) ||
      is.complex(epsilon) || !is.null(dim(epsilon)) ||
      length(epsilon)!=1L || !is.finite(epsilon) ||
      epsilon<=0 || epsilon>1) {
    stop("invalid normalization values or epsilon")
  }
  x <- as.double(x);gamma <- as.double(gamma);beta <- as.double(beta)
  size <- (channels%/%groups)*spatial_size
  result <- numeric(channels*spatial_size)
  for(g in seq_len(groups)) {
    start <- (g-1L)*size+1L
    stop <- start+size-1L
    values <- x[start:stop]
    mu <- sum(values/size)
    variance <- sum((values-mu)^2/size)
    denom <- sqrt(variance+epsilon)
    if (!is.finite(denom) || denom<=0) stop("invalid denominator")
    for(i in start:stop) {
      channel <- (i-1L)%/%spatial_size+1L
      output <- gamma[[channel]]*((x[[i]]-mu)/denom)+beta[[channel]]
      if (!is.finite(output)) stop("nonfinite output")
      result[[i]] <- output
    }
  }
  as.double(result)
}
