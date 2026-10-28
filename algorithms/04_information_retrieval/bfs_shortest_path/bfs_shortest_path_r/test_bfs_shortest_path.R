file_arg <- grep("^--file=",commandArgs(FALSE),value=TRUE)
if (length(file_arg)==0L) stop("run with Rscript")
folder <- dirname(normalizePath(sub("^--file=","",file_arg[[1L]])))
source(file.path(folder,"bfs_shortest_path.R"))
expect_error <- function(expr) {
  failed <- tryCatch({force(expr);FALSE},error=function(e) TRUE)
  stopifnot(failed)
}
graph <- matrix(c(
  0,1,1,0,0,0, 0,0,0,1,0,0, 0,0,0,1,0,0,
  0,0,0,0,1,0, 0,0,0,0,0,0, 0,0,0,0,0,0
),nrow=6,byrow=TRUE)
stopifnot(identical(bfs_shortest_path(graph,0),c(0L,1L,1L,2L,3L,-1L)))
stopifnot(identical(bfs_shortest_path(graph,5),c(-1L,-1L,-1L,-1L,-1L,0L)))
stopifnot(identical(bfs_shortest_path(matrix(1,nrow=1),0),0L))
stopifnot(identical(bfs_shortest_path(matrix(0,nrow=1),0),0L))
expect_error(bfs_shortest_path(matrix(numeric(),nrow=0,ncol=0),0))
expect_error(bfs_shortest_path(matrix(0,nrow=2,ncol=3),0))
expect_error(bfs_shortest_path(matrix(0,nrow=2,ncol=2),2))
expect_error(bfs_shortest_path(matrix(c(0,2,0,0),nrow=2),0))
expect_error(bfs_shortest_path(matrix(c(0,-1,0,0),nrow=2),0))
expect_error(bfs_shortest_path(matrix(c(0,NaN,0,0),nrow=2),0))
expect_error(bfs_shortest_path(matrix(c(0,0.5,0,0),nrow=2),0))
expect_error(bfs_shortest_path(matrix("1",nrow=1),0))
cat("R BFS shortest-path tests passed\n")
