args <- grep("^--file=",commandArgs(FALSE),value=TRUE)
folder <- dirname(normalizePath(sub("^--file=","",args[[1L]])))
source(file.path(folder,"edmonds_karp_max_flow.R"))
expect_error <- function(expr) stopifnot(tryCatch({force(expr);FALSE},error=function(e)TRUE))
network <- function(n,edges) {
  a <- integer(n*n)
  if (nrow(edges)>0L) for (i in seq_len(nrow(edges))) {
    u <- edges[i,1L]+1L
    v <- edges[i,2L]+1L
    a[[(u-1L)*n+v]] <- as.integer(edges[i,3L])
  }
  a
}
edges <- function(...) {
  values <- c(...)
  if (length(values)==0L) return(matrix(integer(0),ncol=3L))
  matrix(values,ncol=3L,byrow=TRUE)
}
check <- function(a,n,s,t,want) {
  stopifnot(identical(edmonds_karp_max_flow(a,n,s,t),as.integer(want)))
}
classic <- network(6,edges(0,1,16,0,2,13,1,2,10,2,1,4,
                           1,3,12,2,4,14,3,2,9,4,3,7,3,5,20,4,5,4))
check(classic,6,0,5,23)
check(network(2,edges(0,1,7)),2,0,1,7)
check(network(2,edges(0,1,7)),2,1,0,0)
check(network(3,edges(0,1,4,1,2,2,0,2,1)),3,0,2,3)
check(network(3,edges(0,1,4,1,0,3,1,2,2,0,2,1)),3,0,2,3)
check(network(4,edges(0,1,6,2,3,7)),4,0,3,0)
check(network(3,edges()),3,0,2,0)
large <- integer(16*16)
for (i in 1:14) {
  large[[i+1L]] <- 1000L
  large[[(i)*16L+16L]] <- 1000L
}
check(large,16,0,15,14000)
expect_error(edmonds_karp_max_flow(integer(0),1,0,0))
expect_error(edmonds_karp_max_flow(integer(0),17,0,1))
expect_error(edmonds_karp_max_flow(integer(0),2,0,1))
expect_error(edmonds_karp_max_flow(c(0,1,0,0),2,0,0))
expect_error(edmonds_karp_max_flow(c(0,1,0,0),2,0,2))
expect_error(edmonds_karp_max_flow(c(0,1,0,0),2,TRUE,1))
expect_error(edmonds_karp_max_flow(c(0,1,0,0),2,0,1.2))
expect_error(edmonds_karp_max_flow(c(1,0,0,0),2,0,1))
expect_error(edmonds_karp_max_flow(c(0,1001,0,0),2,0,1))
expect_error(edmonds_karp_max_flow(c(0,-1,0,0),2,0,1))
expect_error(edmonds_karp_max_flow(c(0,0.5,0,0),2,0,1))
expect_error(edmonds_karp_max_flow(c(FALSE,TRUE,FALSE,FALSE),2,0,1))
expect_error(edmonds_karp_max_flow(matrix(0,2,2),2,0,1))
cat("R max flow tests passed\n")
