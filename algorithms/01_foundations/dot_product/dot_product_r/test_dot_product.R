file_arg <- grep("^--file=", commandArgs(FALSE), value = TRUE)
if (length(file_arg) == 0L) stop("run with Rscript")
folder <- dirname(normalizePath(sub("^--file=", "", file_arg[[1L]])))
source(file.path(folder, "dot_product.R"))

expect_error <- function(expr) {
  did_error <- tryCatch({ force(expr); FALSE }, error = function(e) TRUE)
  stopifnot(did_error)
}

stopifnot(identical(dot_product(c(1, 2, 3), c(4, 5, 6)), 32.0))
stopifnot(identical(dot_product(c(-1, 2, -3), c(4, -5, 6)), -32.0))
stopifnot(identical(dot_product(numeric(), numeric()), 0.0))
expect_error(dot_product(c(1), numeric()))
expect_error(dot_product(c(NaN), c(1)))
expect_error(dot_product(c(Inf), c(1)))
expect_error(dot_product(c(1e308), c(1e308)))
cat("R dot_product tests passed\n")
