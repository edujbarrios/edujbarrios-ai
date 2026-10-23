file_arg <- grep("^--file=", commandArgs(FALSE), value = TRUE)
if (length(file_arg) == 0L) stop("run with Rscript")
folder <- dirname(normalizePath(sub("^--file=", "", file_arg[[1L]])))
source(file.path(folder, "euclidean_distance.R"))

expect_error <- function(expr) {
  did_error <- tryCatch({ force(expr); FALSE }, error = function(e) TRUE)
  stopifnot(did_error)
}

stopifnot(identical(euclidean_distance(c(0, 0), c(3, 4)), 5.0))
stopifnot(identical(euclidean_distance(c(1, 2), c(1, 2)), 0.0))
stopifnot(identical(euclidean_distance(numeric(), numeric()), 0.0))
stopifnot(abs(euclidean_distance(c(1e154), c(0)) / 1e154 - 1) < 1e-12)
expect_error(euclidean_distance(c(1), numeric()))
expect_error(euclidean_distance(c(NaN), c(0)))
expect_error(euclidean_distance(c(Inf), c(0)))
expect_error(euclidean_distance(c(1e308), c(-1e308)))
cat("R euclidean_distance tests passed\n")
