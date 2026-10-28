file_arg <- grep("^--file=",commandArgs(FALSE),value=TRUE)
if (length(file_arg)==0L) stop("run with Rscript")
folder <- dirname(normalizePath(sub("^--file=","",file_arg[[1L]])))
source(file.path(folder,"undirected_connected_components.R"))
err <- function(expr) stopifnot(tryCatch({force(expr);FALSE},error=function(e) TRUE))
graph <- matrix(c(
  0,0,1,0,0,0, 0,0,0,0,1,0,
  1,0,0,1,0,0, 0,0,1,0,0,0,
  0,1,0,0,0,0, 0,0,0,0,0,0
),nrow=6,byrow=TRUE)
stopifnot(identical(undirected_connected_components(graph),c(0L,1L,0L,0L,1L,5L)))
stopifnot(identical(undirected_connected_components(matrix(c(1,1,1,0),2,byrow=TRUE)),c(0L,0L)))
stopifnot(identical(undirected_connected_components(matrix(0,2,2)),c(0L,1L)))
stopifnot(identical(undirected_connected_components(matrix(1,nrow=1)),0L))
err(undirected_connected_components(matrix(numeric(),0,0)))
err(undirected_connected_components(matrix(0,2,3)))
err(undirected_connected_components(matrix(c(0,1,0,0),2,byrow=TRUE)))
err(undirected_connected_components(matrix(c(0,2,2,0),2)))
err(undirected_connected_components(matrix(c(0,NaN,0,0),2)))
err(undirected_connected_components(matrix(c(0,0.5,0.5,0),2)))
err(undirected_connected_components(matrix("1",1,1)))
cat("R undirected components tests passed\n")
