# Rotary positional embeddings with adjacent pairs for query and key.
rotary_positional_embedding <- function(query,key,query_position,key_position) {
  valid_pos<-function(p) {
    is.numeric(p) && !is.complex(p) && length(p)==1L &&
      is.finite(p) && p==floor(p) && p>=0 && p<=1000000
  }
  if (!is.numeric(query) || is.complex(query) ||
      !is.numeric(key) || is.complex(key) ||
      length(query)!=length(key) || length(query)<2L ||
      length(query)>1024L || length(query)%%2L!=0L ||
      any(!is.finite(query)) || any(!is.finite(key)) ||
      !valid_pos(query_position) || !valid_pos(key_position)) {
    stop("invalid even dimensions, finite inputs or positions")
  }
  d<-length(query)
  qr<-numeric(d)
  kr<-numeric(d)
  for (j in seq.int(1L,d,by=2L)) {
    frequency<-10000.0^((j-1L)/d)
    theta_q<-query_position/frequency
    theta_k<-key_position/frequency
    cq<-cos(theta_q);sq<-sin(theta_q)
    ck<-cos(theta_k);sk<-sin(theta_k)
    qr[[j]]<-query[[j]]*cq-query[[j+1L]]*sq
    qr[[j+1L]]<-query[[j]]*sq+query[[j+1L]]*cq
    kr[[j]]<-key[[j]]*ck-key[[j+1L]]*sk
    kr[[j+1L]]<-key[[j]]*sk+key[[j+1L]]*ck
    if (any(!is.finite(c(qr[[j]],qr[[j+1L]],kr[[j]],kr[[j+1L]])))) {
      stop("rotary coordinate overflow")
    }
  }
  list(query=qr,key=kr)
}
