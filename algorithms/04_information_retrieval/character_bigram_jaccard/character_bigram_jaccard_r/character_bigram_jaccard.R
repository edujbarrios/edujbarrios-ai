# Binary Jaccard similarity over sets of printable ASCII character pairs.
character_bigram_jaccard <- function(a,b) {
  checked <- function(value) {
    if (!is.character(value) || length(value)!=1L || is.na(value)) {
      stop("expected one printable ASCII string")
    }
    chars <- utf8ToInt(enc2utf8(value))
    if (length(chars)>512L || any(chars<32L) || any(chars>126L)) {
      stop("expected printable ASCII of length at most 512")
    }
    chars
  }
  left <- checked(a)
  right <- checked(b)
  present_left <- logical(95L*95L)
  present_right <- logical(95L*95L)
  if (length(left)>=2L) {
    for (i in seq_len(length(left)-1L)) {
      key <- (left[[i]]-32L)*95L+(left[[i+1L]]-32L)+1L
      present_left[[key]] <- TRUE
    }
  }
  if (length(right)>=2L) {
    for (i in seq_len(length(right)-1L)) {
      key <- (right[[i]]-32L)*95L+(right[[i+1L]]-32L)+1L
      present_right[[key]] <- TRUE
    }
  }
  union_count <- sum(present_left | present_right)
  if (union_count==0L) return(1.0)
  as.double(sum(present_left & present_right)/union_count)
}
