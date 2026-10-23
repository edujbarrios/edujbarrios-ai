args<-grep("^--file=",commandArgs(FALSE),value=TRUE)
folder<-dirname(normalizePath(sub("^--file=","",args[[1L]])))
source(file.path(folder,"dense_relu_backprop.R"))
expect_error<-function(expr) stopifnot(tryCatch({force(expr);FALSE},error=function(e)TRUE))
w<-matrix(c(1,1,-1,2),nrow=2,byrow=TRUE)
result<-dense_relu_backprop(c(2,-1),w,c(0,1),c(3,4))
stopifnot(identical(result$grad_input,c(3,3)))
stopifnot(identical(unname(result$grad_weights),
                    matrix(c(6,-3,0,0),nrow=2,byrow=TRUE)))
stopifnot(identical(result$grad_bias,c(3,0)))
zero<-dense_relu_backprop(c(0),matrix(3,nrow=1),c(0),c(5))
stopifnot(identical(zero$grad_input,c(0)),
          identical(unname(zero$grad_weights),matrix(0,nrow=1)),
          identical(zero$grad_bias,c(0)))
expect_error(dense_relu_backprop(numeric(),w,c(0,1),c(3,4)))
expect_error(dense_relu_backprop(c(1),w,c(0,1),c(3,4)))
expect_error(dense_relu_backprop(c(1),matrix(NaN,1),c(0),c(1)))
expect_error(dense_relu_backprop(c(1e308),matrix(1e308,1),c(0),c(1)))
cat("R dense ReLU backprop tests passed\n")
