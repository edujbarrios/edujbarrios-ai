# Closed-form principal component of two-feature real sample matrices.
pca_2d <- function(points) {
  if (!is.matrix(points) || !is.numeric(points) || is.complex(points) ||
      nrow(points)<2L || ncol(points)!=2L || any(!is.finite(points))) {
    stop("expected finite n x 2 matrix, n>=2")
  }
  n <- nrow(points)
  mx<-0.0;my<-0.0
  for (i in seq_len(n)) {
    dx<-as.double(points[i,1])-mx;dy<-as.double(points[i,2])-my
    if(any(!is.finite(c(dx,dy)))) stop("mean overflow")
    mx<-mx+dx/i;my<-my+dy/i
    if(any(!is.finite(c(mx,my)))) stop("mean overflow")
  }
  a<-0.0;b<-0.0;c<-0.0
  for (i in seq_len(n)) {
    dx<-as.double(points[i,1])-mx;dy<-as.double(points[i,2])-my
    xx<-dx*dx;xy<-dx*dy;yy<-dy*dy
    if(any(!is.finite(c(xx,xy,yy)))) stop("covariance overflow")
    a<-a+xx;b<-b+xy;c<-c+yy
    if(any(!is.finite(c(a,b,c)))) stop("covariance overflow")
  }
  a<-a/n;b<-b/n;c<-c/n
  if(a==0 && c==0) stop("zero covariance")
  if(b==0) {
    if(a>=c) {vx<-1.0;vy<-0.0} else {vx<-0.0;vy<-1.0}
  } else {
    delta<-a-c;twice_b<-2*b
    disc<-sqrt(delta^2+twice_b^2)
    vx<-delta+disc;vy<-twice_b
    length<-sqrt(vx^2+vy^2)
    if(any(!is.finite(c(delta,twice_b,disc,vx,length))) || length<=0)
      stop("eigenvector overflow")
    vx<-vx/length;vy<-vy/length
    if(vx<0 || (vx==0 && vy<0)) {vx<--vx;vy<--vy}
  }
  projections<-numeric(n)
  for (i in seq_len(n)) {
    dx<-as.double(points[i,1])-mx;dy<-as.double(points[i,2])-my
    px<-dx*vx;py<-dy*vy
    projections[[i]]<-px+py
    if(any(!is.finite(c(dx,dy,px,py,projections[[i]]))))
      stop("projection overflow")
  }
  list(mean=c(mx,my),direction=c(vx,vy),projections=projections)
}
