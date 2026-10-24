# Search packed 30-bit signatures by Hamming radius (0 through 30).
hamming_radius_search <- function(query,signatures,radius) {
  valid <- function(x) {
    is.numeric(x) && !is.complex(x) && all(is.finite(x)) &&
      all(x==floor(x)) && all(x>=0) && all(x<=2^30-1)
  }
  if (!valid(query) || length(query)!=1L ||
      !valid(signatures) || !is.numeric(radius) ||
      is.complex(radius) || length(radius)!=1L ||
      !is.finite(radius) || radius!=floor(radius) ||
      radius<0 || radius>30) {
    stop("invalid signatures or radius")
  }
  distances<-integer(length(signatures))
  for (i in seq_along(signatures)) {
    bits<-bitwXor(as.integer(query),as.integer(signatures[[i]]))
    count<-0L
    for (b in seq_len(30)) {
      count<-count+bitwAnd(bits,1L)
      bits<-bitwShiftR(bits,1L)
    }
    distances[[i]]<-count
  }
  matches<-which(distances<=radius)
  if (length(matches)==0L) return(integer())
  as.integer(matches[order(distances[matches],matches)]-1L)
}
