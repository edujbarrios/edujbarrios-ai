args <- grep("^--file=",commandArgs(FALSE),value=TRUE)
folder <- dirname(normalizePath(sub("^--file=","",args[[1L]])))
source(file.path(folder,"undirected_graph_bridges.R"))
expect_error <- function(expr) stopifnot(tryCatch({force(expr);FALSE},error=function(e)TRUE))
edges <- function(...) {
  values <- c(...)
  if (length(values)==0L) return(matrix(integer(0),ncol=2L))
  matrix(values,ncol=2L,byrow=TRUE)
}
graph <- function(n,es) {
  a <- integer(n*n)
  if (nrow(es)>0L) for (i in seq_len(nrow(es))) {
    u <- es[i,1]+1L
    v <- es[i,2]+1L
    a[[(u-1L)*n+v]] <- 1L
    a[[(v-1L)*n+u]] <- 1L
  }
  a
}
check <- function(a,n,want) {
  stopifnot(identical(undirected_graph_bridges(a,n),as.integer(want)))
}
check(graph(4,edges(0,1,1,2,2,3)),4,c(0,1,1,2,2,3))
check(graph(4,edges(0,1,1,2,2,3,3,0)),4,integer(0))
check(graph(4,edges(0,1,0,2,0,3)),4,c(0,1,0,2,0,3))
check(graph(5,edges(0,1,1,2,2,0,2,3,3,4)),5,c(2,3,3,4))
check(graph(7,edges(0,1,1,2,3,4,4,5,5,3)),7,c(0,1,1,2))
check(graph(4,edges(0,1,0,2,0,3,1,2,1,3,2,3)),4,integer(0))
check(graph(1,edges()),1,integer(0))
check(graph(2,edges(0,1)),2,c(0,1))
check(graph(3,edges()),3,integer(0))
check(graph(64,cbind(0:62,1:63)),64,as.vector(rbind(0:62,1:63)))
expect_error(undirected_graph_bridges(integer(0),0))
expect_error(undirected_graph_bridges(0,65))
expect_error(undirected_graph_bridges(0,TRUE))
expect_error(undirected_graph_bridges(0,1.5))
expect_error(undirected_graph_bridges(integer(0),1))
expect_error(undirected_graph_bridges(1,1))
expect_error(undirected_graph_bridges(c(0,1,0,0),2))
expect_error(undirected_graph_bridges(c(0,2,2,0),2))
expect_error(undirected_graph_bridges(c(FALSE,TRUE,TRUE,FALSE),2))
expect_error(undirected_graph_bridges(matrix(0,1,1),1))
cat("R undirected graph bridges tests passed\n")
