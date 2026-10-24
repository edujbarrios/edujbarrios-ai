# Two-term positional phrase search via sorted postings matrix intersection.
positional_phrase_search <- function(first,second) {
  validate <- function(x) {
    if (!is.matrix(x) || !is.numeric(x) || is.complex(x) ||
        ncol(x)!=2L || nrow(x)>4096L || any(!is.finite(x)) ||
        any(x!=floor(x)) || any(x<0) || any(x>1000000000)) {
      stop("positional postings must be integer-valued n x 2 matrices")
    }
    if (nrow(x)>1L) for (i in seq.int(2L,nrow(x))) {
      if (x[i,1]<x[i-1L,1] ||
          (x[i,1]==x[i-1L,1] && x[i,2]<=x[i-1L,2])) {
        stop("postings must be strictly sorted by document and position")
      }
    }
    TRUE
  }
  validate(first);validate(second)
  a<-1L;b<-1L
  na<-nrow(first);nb<-nrow(second)
  output<-integer()
  while (a<=na && b<=nb) {
    da<-as.double(first[a,1]);db<-as.double(second[b,1])
    if (da<db) {
      while (a<=na && first[a,1]==da) a<-a+1L
    } else if (db<da) {
      while (b<=nb && second[b,1]==db) b<-b+1L
    } else {
      ea<-a;eb<-b
      while (ea<=na && first[ea,1]==da) ea<-ea+1L
      while (eb<=nb && second[eb,1]==db) eb<-eb+1L
      pa<-a;pb<-b
      while (pa<ea && pb<eb) {
        wanted<-as.double(first[pa,2])+1
        observed<-as.double(second[pb,2])
        if (wanted==observed) {
          output<-c(output,as.integer(da))
          break
        }
        if (wanted<observed) pa<-pa+1L else pb<-pb+1L
      }
      a<-ea;b<-eb
    }
  }
  as.integer(output)
}
