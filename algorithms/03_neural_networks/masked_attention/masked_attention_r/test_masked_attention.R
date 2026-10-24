args<-grep("^--file=",commandArgs(FALSE),value=TRUE)
folder<-dirname(normalizePath(sub("^--file=","",args[[1L]])))
source(file.path(folder,"masked_attention.R"))
expect_error<-function(expr) stopifnot(tryCatch({force(expr);FALSE},error=function(e)TRUE))
keys<-matrix(c(1,0,0,1,1,1),nrow=3,byrow=TRUE)
values<-matrix(c(10,0,0,20,4,6),nrow=3,byrow=TRUE)
result<-masked_attention(c(1,0),keys,values,c(1,0,1))
stopifnot(max(abs(result-c(7,3)))<1e-12)
stopifnot(identical(masked_attention(c(1,0),keys,values,c(0,1,0)),c(0,20)))
stopifnot(identical(masked_attention(c(1e308),
  matrix(c(1e308,1),ncol=1),matrix(c(0,7),ncol=1),c(0,1)),c(7)))
expect_error(masked_attention(c(1,0),keys,values,c(0,0,0)))
expect_error(masked_attention(c(1,0),keys,values,c(1,2,0)))
expect_error(masked_attention(c(1,0),keys,values,c(1,0)))
expect_error(masked_attention(c(1e308),
  matrix(c(1e308,1),ncol=1),matrix(c(0,7),ncol=1),c(1,0)))
expect_error(masked_attention(c(1),
  matrix(c(NaN,1),ncol=1),matrix(c(0,7),ncol=1),c(0,1)))
cat("R masked_attention tests passed\n")
