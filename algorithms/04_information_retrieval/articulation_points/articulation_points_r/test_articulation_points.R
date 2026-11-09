args <- grep("^--file=",commandArgs(FALSE),value=TRUE)
folder <- dirname(normalizePath(sub("^--file=","",args[[1L]])))
source(file.path(folder,"articulation_points.R"))
expect_error <- function(expr) stopifnot(tryCatch({force(expr);FALSE},error=function(e)TRUE))
graph <- function(n,edges) {
  result <- integer(n*n)
  if (nrow(edges)>0L) for (i in seq_len(nrow(edges))) {
    u <- edges[[i,1L]]+1L
    v <- edges[[i,2L]]+1L
    result[[(u-1L)*n+v]] <- 1L
    result[[(v-1L)*n+u]] <- 1L
  }
  result
}
edges <- function(...) {
  values <- c(...)
  if (length(values)==0L) return(matrix(integer(0),ncol=2L))
  matrix(values,ncol=2L,byrow=TRUE)
}
check <- function(a,n,want) stopifnot(identical(articulation_points(a,n),as.integer(want)))
check(graph(5,edges(0,1, 1,2, 2,3, 3,4)),5,c(1,2,3))
check(graph(4,edges(0,1, 1,2, 2,3, 3,0)),4,integer(0))
check(graph(5,edges(0,1, 0,2, 0,3, 0,4)),5,0)
check(graph(6,edges(0,1, 1,2, 3,4)),6,1)
check(graph(4,edges(0,1, 0,2, 0,3, 1,2, 1,3, 2,3)),4,integer(0))
check(graph(1,edges()),1,integer(0))
check(graph(2,edges(0,1)),2,integer(0))
check(graph(3,edges()),3,integer(0))
check(graph(64,cbind(0:62,1:63)),64,1:62)
expect_error(articulation_points(integer(0),0))
expect_error(articulation_points(0,65))
expect_error(articulation_points(0,TRUE))
expect_error(articulation_points(0,1.0+0.1))
expect_error(articulation_points(integer(0),1))
expect_error(articulation_points(c(0,1),1))
expect_error(articulation_points(1,1))
expect_error(articulation_points(c(0,1,0,0),2))
expect_error(articulation_points(c(0,2,2,0),2))
expect_error(articulation_points(c(0,TRUE,TRUE,0),2))
expect_error(articulation_points(matrix(0,1,1),1))
cat("R articulation points tests passed\n")
