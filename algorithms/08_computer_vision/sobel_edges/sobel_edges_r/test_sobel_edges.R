file_arg <- grep("^--file=",commandArgs(FALSE),value=TRUE)
if (length(file_arg)==0L) stop("run with Rscript")
folder <- dirname(normalizePath(sub("^--file=","",file_arg[[1L]])))
source(file.path(folder,"sobel_edges.R"))
expect_error <- function(expr) {
  failed <- tryCatch({force(expr);FALSE},error=function(e) TRUE)
  stopifnot(failed)
}
stopifnot(identical(sobel_edges(c(0,1,2,0,1,2,0,1,2),3,3),
                    c(4,8,4,4,8,4,4,8,4)))
stopifnot(identical(sobel_edges(c(0,0,0,1,1,1,2,2,2),3,3),
                    c(4,4,4,8,8,8,4,4,4)))
stopifnot(identical(sobel_edges(rep(5,4),2,2),rep(0,4)))
stopifnot(identical(sobel_edges(7,1,1),0))
expect_error(sobel_edges(c(0,1,2,0,1,2,0,1,2),0,3))
expect_error(sobel_edges(c(0,1,2,0,1,2,0,1,2),65,3))
expect_error(sobel_edges(c(0,1,2,0,1,2,0,1,2),3,0))
expect_error(sobel_edges(c(0,1,2),3,3))
expect_error(sobel_edges(c(NaN),1,1))
expect_error(sobel_edges(c(1001),1,1))
expect_error(sobel_edges(c(Inf),1,1))
cat("R Sobel edge tests passed\n")
