args <- grep("^--file=", commandArgs(FALSE), value=TRUE)
if (length(args) == 0L) stop("run with Rscript")
folder <- dirname(normalizePath(sub("^--file=", "", args[[1L]])))
source(file.path(folder, "binary_closing.R"))
error <- function(expr) stopifnot(tryCatch({force(expr); FALSE}, error=function(e) TRUE))
image <- integer(49)
expected <- integer(49)
for (y in 3:5) for (x in 3:5) {
  image[(y-1L)*7L+x] <- 1L
  expected[(y-1L)*7L+x] <- 1L
}
image[(4L-1L)*7L+4L] <- 0L
stopifnot(identical(binary_closing(image, 7, 7), expected))
stopifnot(identical(binary_closing(1,1,1), 0L))
stopifnot(identical(binary_closing(0,1,1), 0L))
error(binary_closing(numeric(),1,1))
error(binary_closing(1,0,1))
error(binary_closing(1,1,65))
error(binary_closing(2,1,1))
error(binary_closing(TRUE,1,1))
error(binary_closing(1,TRUE,1))
cat("R binary closing tests passed\n")
