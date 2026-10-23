args<-grep("^--file=",commandArgs(FALSE),value=TRUE)
folder<-dirname(normalizePath(sub("^--file=","",args[[1L]])))
source(file.path(folder,"confusion_matrix.R"))
expect_error<-function(expr) stopifnot(tryCatch({force(expr);FALSE},error=function(e)TRUE))
stopifnot(identical(confusion_matrix(c(0,0,1,1,1),c(0,1,1,0,1)),
                    c(TN=1L,FP=1L,FN=1L,TP=2L)))
stopifnot(identical(confusion_matrix(c(0,1),c(0,1)),
                    c(TN=1L,FP=0L,FN=0L,TP=1L)))
stopifnot(identical(confusion_matrix(c(0,0),c(0,0)),
                    c(TN=2L,FP=0L,FN=0L,TP=0L)))
expect_error(confusion_matrix(numeric(),numeric()))
expect_error(confusion_matrix(c(0),c(0,1)))
expect_error(confusion_matrix(c(0,2),c(0,1)))
expect_error(confusion_matrix(c(0,1),c(0,NaN)))
cat("R confusion matrix tests passed\n")
