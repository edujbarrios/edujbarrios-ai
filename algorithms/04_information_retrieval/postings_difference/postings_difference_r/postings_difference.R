# Boolean AND NOT of strictly sorted unique document IDs.
postings_difference <- function(included,excluded) {
  validate <- function(values) {
    if (!is.numeric(values) || is.complex(values) ||
        any(!is.finite(values)) || any(values!=floor(values)) ||
        any(values<0) || any(values>2147483647) ||
        (length(values)>1L && any(diff(as.double(values))<=0))) {
      stop("postings must contain sorted unique IDs in 0..2^31-1")
    }
    as.integer(values)
  }
  a<-validate(included)
  b<-validate(excluded)
  i<-1L;j<-1L
  result<-integer()
  while (i<=length(a) && j<=length(b)) {
    if (a[[i]]<b[[j]]) {
      result<-c(result,a[[i]])
      i<-i+1L
    } else if (a[[i]]>b[[j]]) {
      j<-j+1L
    } else {
      i<-i+1L
      j<-j+1L
    }
  }
  if (i<=length(a)) result<-c(result,a[i:length(a)])
  as.integer(result)
}
