args<-grep("^--file=",commandArgs(FALSE),value=TRUE)
folder<-dirname(normalizePath(sub("^--file=","",args[[1L]])))
source(file.path(folder,"causal_attention.R"))
expect_error<-function(expr) stopifnot(tryCatch({force(expr);FALSE},error=function(e)TRUE))
q<-matrix(c(0,0,0),ncol=1)
k<-matrix(c(1,2,3),ncol=1)
v<-matrix(c(2,4,6,8,10,12),nrow=3,byrow=TRUE)
out<-causal_attention(q,k,v)
expected<-matrix(c(2,4,4,6,6,8),nrow=3,byrow=TRUE)
stopifnot(max(abs(out-expected))<1e-12)
stopifnot(identical(causal_attention(matrix(3,1,1),
                                    matrix(3,1,1),matrix(7,1,1)),
                    matrix(7,nrow=1)))
out<-causal_attention(matrix(c(1000,1000),ncol=1),
  matrix(c(-1000,1000),ncol=1),matrix(c(0,7),ncol=1))
stopifnot(identical(out,matrix(c(0,7),ncol=1)))
expect_error(causal_attention(matrix(NaN,1,1),matrix(1,1,1),matrix(1,1,1)))
expect_error(causal_attention(matrix(1e308,1,1),
                              matrix(1e308,1,1),matrix(1,1,1)))
expect_error(causal_attention(matrix(1,1,1),matrix(1,2,1),matrix(1,1,1)))
expect_error(causal_attention(matrix(numeric(),ncol=1),
                              matrix(numeric(),ncol=1),matrix(numeric(),ncol=1)))
cat("R causal attention tests passed\n")
