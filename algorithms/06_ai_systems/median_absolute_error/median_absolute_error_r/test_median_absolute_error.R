args <- grep("^--file=", commandArgs(FALSE), value=TRUE)
folder <- dirname(normalizePath(sub("^--file=", "", args[[1L]])))
source(file.path(folder, "median_absolute_error.R"))
expect_error <- function(expr) {
  stopifnot(tryCatch({force(expr); FALSE}, error=function(e) TRUE))
}
medae <- median_absolute_error
stopifnot(medae(c(0,10,0), c(0,0,0)) == 0)
stopifnot(medae(c(1,2,3,4), c(0,0,0,0)) == 2.5)
stopifnot(medae(c(1,2), c(1,2)) == 0)
stopifnot(medae(c(-1,-3,4), c(0,0,0)) == 3)
stopifnot(medae(5, -5) == 10)
stopifnot(medae(c(1e308,1e308), c(0,0)) == 1e308)
expect_error(medae(numeric(), numeric()))
expect_error(medae(1, c(1,2)))
expect_error(medae(TRUE, 1))
expect_error(medae(NaN, 1))
expect_error(medae(1e308, -1e308))
cat("R median absolute error tests passed\n")
