# Linear-time OR merge for strictly increasing document ID arrays.
postings_union <- function(left,right) {
  validate <- function(values) {
    if (!is.numeric(values) || is.complex(values) ||
        any(!is.finite(values)) || any(values!=floor(values)) ||
        any(values<0) || any(values>2147483647) ||
        (length(values)>1L && any(diff(as.double(values))<=0))) {
      stop("postings IDs must be strictly increasing integers in 0..2^31-1")
    }
    as.integer(values)
  }
  a<-validate(left);b<-validate(right)
  i<-1L;j<-1L
  result<-integer()
  while (i<=length(a) && j<=length(b)) {
    if (a[[i]]<b[[j]]) {
      result<-c(result,a[[i]])
      i<-i+1L
    } else if (a[[i]]>b[[j]]) {
      result<-c(result,b[[j]])
      j<-j+1L
    } else {
      result<-c(result,a[[i]])
      i<-i+1L;j<-j+1L
    }
  }
  if (i<=length(a)) result<-c(result,a[i:length(a)])
  if (j<=length(b)) result<-c(result,b[j:length(b)])
  as.integer(result)
}
