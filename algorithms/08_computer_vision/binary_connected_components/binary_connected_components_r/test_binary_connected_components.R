args <- grep("^--file=", commandArgs(FALSE), value=TRUE)
folder <- dirname(normalizePath(sub("^--file=", "", args[[1L]])))
source(file.path(folder, "binary_connected_components.R"))
expect_error <- function(expr) stopifnot(tryCatch({force(expr); FALSE}, error=function(e) TRUE))
stopifnot(identical(binary_connected_components(c(1,1,0,0,1,0,0,0,1),3,3),
                    list(labels=c(1L,1L,0L,0L,1L,0L,0L,0L,2L),count=2L)))
stopifnot(identical(binary_connected_components(c(1,0,0,1),2,2),
                    list(labels=c(1L,0L,0L,2L),count=2L)))
stopifnot(identical(binary_connected_components(rep(1,4),2,2),
                    list(labels=rep(1L,4),count=1L)))
stopifnot(identical(binary_connected_components(rep(0,4),2,2),
                    list(labels=integer(4),count=0L)))
stopifnot(identical(binary_connected_components(c(1,0,1),1,3),
                    list(labels=c(1L,0L,2L),count=2L)))
stopifnot(identical(binary_connected_components(1,1,1),
                    list(labels=1L,count=1L)))
checker <- as.integer(unlist(lapply(0:63, function(row) (row + 0:63) %% 2L)))
answer <- binary_connected_components(checker,64,64)
stopifnot(identical(answer$count,2048L))
stopifnot(identical(answer$labels[answer$labels>0], as.integer(1:2048)))
expect_error(binary_connected_components(1,0,1))
expect_error(binary_connected_components(1,1,65))
expect_error(binary_connected_components(c(0,2),1,2))
expect_error(binary_connected_components(c(1,0),1,1))
expect_error(binary_connected_components(c(TRUE,FALSE),1,2))
expect_error(binary_connected_components(matrix(1:4,2,2),2,2))
expect_error(binary_connected_components(c(1,NA),1,2))
cat("R connected-component tests passed\n")
