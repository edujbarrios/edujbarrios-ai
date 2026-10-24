# Static sorted LSH bucket index over unsigned 30-bit signatures.
valid_signatures <- function(x) {
  is.numeric(x) && !is.complex(x) && all(is.finite(x)) &&
    all(x==floor(x)) && all(x>=0) && all(x<=2^30-1)
}
build_lsh_bucket_index <- function(signatures) {
  if (!valid_signatures(signatures) || length(signatures)==0L) {
    stop("nonempty unsigned 30-bit signatures required")
  }
  ordering<-order(signatures,seq_along(signatures))
  list(signatures=as.double(signatures[ordering]),
       indices=as.integer(ordering-1L))
}
lsh_bucket_index_query <- function(index, signature) {
  if (!is.list(index) || is.null(index$signatures) ||
      is.null(index$indices) || !valid_signatures(signature) ||
      length(signature)!=1L ||
      !valid_signatures(index$signatures) ||
      length(index$indices)!=length(index$signatures) ||
      length(index$signatures)==0L ||
      any(diff(index$signatures)<0)) {
    stop("invalid index or 30-bit query signature")
  }
  keys<-index$signatures
  n<-length(keys)
  lo<-1L;hi<-n+1L
  while(lo<hi) {
    mid<-lo+(hi-lo)%/%2L
    if(keys[[mid]]<signature) lo<-mid+1L else hi<-mid
  }
  result<-integer()
  while(lo<=n && keys[[lo]]==signature) {
    result<-c(result,as.integer(index$indices[[lo]]))
    lo<-lo+1L
  }
  result
}
