# Rocchio relevance feedback, fixed coefficients and nonnegative clipping.
rocchio_relevance_feedback <- function(query,relevant,nonrelevant) {
  bounded <- function(x) is.numeric(x) && !is.complex(x) &&
    all(is.finite(x)) && all(x>=0) && all(x<=1e6)
  d<-length(query)
  group_ok <- function(x) is.matrix(x) && bounded(x) &&
    nrow(x)<=128L && ncol(x)==d
  if (d<1L || d>64L || !bounded(query) ||
      !group_ok(relevant) || !group_ok(nonrelevant) ||
      nrow(relevant)+nrow(nonrelevant)==0L) {
    stop("invalid finite query or aligned relevance-feedback groups")
  }
  result<-numeric(d)
  for (j in seq_len(d)) {
    pos<-if (nrow(relevant)==0L) 0.0 else
      sum(as.double(relevant[,j])/nrow(relevant))
    neg<-if (nrow(nonrelevant)==0L) 0.0 else
      sum(as.double(nonrelevant[,j])/nrow(nonrelevant))
    value<-as.double(query[[j]])+0.75*pos-0.15*neg
    if (!is.finite(value)) stop("Rocchio numeric overflow")
    result[[j]]<-max(0.0,value)
  }
  result
}
