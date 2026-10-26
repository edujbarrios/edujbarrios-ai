args <- grep("^--file=", commandArgs(FALSE), value=TRUE)
folder <- dirname(normalizePath(sub("^--file=", "", args[[1L]])))
source(file.path(folder, "total_variation_distance.R"))
expect_error <- function(expr) {
  stopifnot(tryCatch({force(expr); FALSE}, error=function(e) TRUE))
}
tv <- total_variation_distance
stopifnot(tv(c(0.5, 0.5), c(0.5, 0.5)) == 0)
stopifnot(tv(c(1, 0), c(0, 1)) == 1)
stopifnot(abs(tv(c(1, 0), c(0.25, 0.75)) - 0.75) < 1e-12)
stopifnot(abs(tv(c(0.25, 0.75), c(1, 0)) - 0.75) < 1e-12)
stopifnot(abs(tv(c(0.1, 0.2, 0.7), c(0.2, 0.3, 0.5)) - 0.2) < 1e-12)
expect_error(tv(numeric(), numeric()))
expect_error(tv(1, c(1, 0)))
expect_error(tv(c(0.4, 0.4), c(0.5, 0.5)))
expect_error(tv(c(1.1, -0.1), c(1, 0)))
expect_error(tv(c(NaN), 1))
expect_error(tv(TRUE, 1))
cat("R total variation tests passed\n")
