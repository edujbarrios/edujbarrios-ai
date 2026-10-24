# Closed-form ridge regression of one feature, with unpenalized intercept.
ridge_regression_1d <- function(x,y,lambda) {
  valid <- function(v) is.numeric(v) && !is.complex(v) &&
    all(is.finite(v)) && all(abs(v)<=1e6)
  n<-length(x)
  if (n<1L || n>4096L || length(y)!=n || !valid(x) || !valid(y) ||
      !is.numeric(lambda) || is.complex(lambda) ||
      length(lambda)!=1L || !is.finite(lambda) ||
      lambda<0 || lambda>1e12) {
    stop("invalid aligned observations or nonnegative penalty")
  }
  mx<-sum(as.double(x)/n)
  my<-sum(as.double(y)/n)
  sxx<-0.0;sxy<-0.0
  for (i in seq_len(n)) {
    dx<-as.double(x[[i]])-mx
    dy<-as.double(y[[i]])-my
    sxx<-sxx+dx*dx
    sxy<-sxy+dx*dy
  }
  denominator<-sxx+lambda
  slope<-if (denominator==0) 0.0 else sxy/denominator
  intercept<-my-slope*mx
  if (any(!is.finite(c(mx,my,sxx,sxy,denominator,slope,intercept)))) {
    stop("ridge regression arithmetic overflow")
  }
  list(intercept=as.double(intercept),slope=as.double(slope))
}
