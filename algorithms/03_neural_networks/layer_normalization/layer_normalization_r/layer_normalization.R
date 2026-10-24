# Single-vector layer normalization with epsilon and per-feature affine.
layer_normalization <- function(input,gamma,beta,epsilon) {
  valid_vector<-function(x) is.numeric(x) && !is.complex(x) &&
    all(is.finite(x))
  n<-length(input)
  if (!valid_vector(input) || n==0L ||
      !valid_vector(gamma) || length(gamma)!=n ||
      !valid_vector(beta) || length(beta)!=n ||
      !valid_vector(epsilon) || length(epsilon)!=1L ||
      epsilon<=0) {
    stop("invalid dimensions, finite values or epsilon")
  }
  mean<-0.0
  for (i in seq_len(n)) {
    delta<-as.double(input[[i]])-mean
    if (!is.finite(delta)) stop("mean overflow")
    mean<-mean+delta/i
    if (!is.finite(mean)) stop("mean overflow")
  }
  squared_sum<-0.0
  for (i in seq_len(n)) {
    delta<-as.double(input[[i]])-mean
    squared<-delta*delta
    if (any(!is.finite(c(delta,squared)))) stop("variance overflow")
    squared_sum<-squared_sum+squared
    if (!is.finite(squared_sum)) stop("variance sum overflow")
  }
  variance<-squared_sum/n
  divisor<-sqrt(variance+as.double(epsilon))
  if (!is.finite(variance) || !is.finite(divisor) || divisor<=0)
    stop("normalizer overflow")
  output<-numeric(n)
  for (i in seq_len(n)) {
    centered<-as.double(input[[i]])-mean
    normalized<-centered/divisor
    weighted<-as.double(gamma[[i]])*normalized
    value<-weighted+as.double(beta[[i]])
    if (any(!is.finite(c(centered,normalized,weighted,value))))
      stop("affine output overflow")
    output[[i]]<-value
  }
  output
}
