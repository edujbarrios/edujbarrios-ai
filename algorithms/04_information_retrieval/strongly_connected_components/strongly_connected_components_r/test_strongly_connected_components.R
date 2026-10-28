file_arg <- grep("^--file=",commandArgs(FALSE),value=TRUE)
if (length(file_arg)==0L) stop("run with Rscript")
folder <- dirname(normalizePath(sub("^--file=","",file_arg[[1L]])))
source(file.path(folder,"strongly_connected_components.R"))
expect_error <- function(expr) {
  failed <- tryCatch({force(expr);FALSE},error=function(e) TRUE)
  stopifnot(failed)
}
graph <- matrix(c(
  0,1,0,0,0,0, 0,0,1,0,0,0, 1,0,0,1,0,0,
  0,0,0,0,1,0, 0,0,0,1,0,1, 0,0,0,0,0,0
),nrow=6,byrow=TRUE)
stopifnot(identical(strongly_connected_components(graph),c(0L,0L,0L,3L,3L,5L)))
stopifnot(identical(strongly_connected_components(matrix(c(0,1,1,0),2,byrow=TRUE)),c(0L,0L)))
stopifnot(identical(strongly_connected_components(matrix(c(0,1,0,0),2,byrow=TRUE)),c(0L,1L)))
stopifnot(identical(strongly_connected_components(matrix(1,nrow=1)),0L))
stopifnot(identical(strongly_connected_components(matrix(0,nrow=1)),0L))
stopifnot(identical(strongly_connected_components(matrix(0,nrow=3,ncol=3)),c(0L,1L,2L)))
expect_error(strongly_connected_components(matrix(numeric(),nrow=0,ncol=0)))
expect_error(strongly_connected_components(matrix(0,nrow=2,ncol=3)))
expect_error(strongly_connected_components(matrix(c(0,2,0,0),nrow=2)))
expect_error(strongly_connected_components(matrix(c(0,-1,0,0),nrow=2)))
expect_error(strongly_connected_components(matrix(c(0,NaN,0,0),nrow=2)))
expect_error(strongly_connected_components(matrix(c(0,0.5,0,0),nrow=2)))
expect_error(strongly_connected_components(matrix("1",nrow=1)))
cat("R SCC tests passed\n")
