args <- grep("^--file=", commandArgs(FALSE), value=TRUE)
folder <- dirname(normalizePath(sub("^--file=", "", args[[1L]])))
source(file.path(folder, "canberra_distance.R"))
expect_error <- function(expr) {
  stopifnot(tryCatch({force(expr); FALSE}, error=function(e) TRUE))
}
d <- canberra_distance
stopifnot(abs(d(c(1,2), c(2,4)) - 2/3) < 1e-12)
stopifnot(d(c(0,0), c(0,0)) == 0)
stopifnot(d(c(1,-1), c(-1,1)) == 2)
stopifnot(d(c(1e308,-1e308), c(-1e308,1e308)) == 2)
stopifnot(abs(d(c(2,4), c(1,2)) - 2/3) < 1e-12)
expect_error(d(numeric(), numeric()))
expect_error(d(1, c(1,2)))
expect_error(d(TRUE, 1))
expect_error(d(NaN, 1))
expect_error(d(Inf, 1))
cat("R Canberra distance tests passed\n")
