# Unit-cost Levenshtein edit distance for printable ASCII strings.
levenshtein_distance <- function(a,b) {
  validated <- function(value) {
    if (!is.character(value) || length(value)!=1L || is.na(value)) {
      stop("expected one printable ASCII string")
    }
    codes <- utf8ToInt(enc2utf8(value))
    if (length(codes)>512L || any(codes<32L) || any(codes>126L)) {
      stop("string must contain <=512 printable ASCII characters")
    }
    codes
  }
  left<-validated(a)
  right<-validated(b)
  if (length(right)>length(left)) {
    tmp<-left;left<-right;right<-tmp
  }
  m<-length(right)
  prev<-as.integer(0:m)
  for (i in seq_along(left)) {
    curr<-integer(m+1L)
    curr[[1L]]<-as.integer(i)
    for (j in seq_along(right)) {
      deletion<-prev[[j+1L]]+1L
      insertion<-curr[[j]]+1L
      substitution<-prev[[j]]+as.integer(left[[i]]!=right[[j]])
      curr[[j+1L]]<-min(deletion,insertion,substitution)
    }
    prev<-curr
  }
  as.integer(prev[[m+1L]])
}
