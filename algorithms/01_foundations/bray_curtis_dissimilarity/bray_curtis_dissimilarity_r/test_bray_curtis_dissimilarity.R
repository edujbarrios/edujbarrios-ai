args <- grep("^--file=", commandArgs(FALSE), value=TRUE)
folder <- dirname(normalizePath(sub("^--file=", "", args[[1L]])))
source(file.path(folder, "bray_curtis_dissimilarity.R"))
expect_error <- function(expr) {
  stopifnot(tryCatch({force(expr); FALSE}, error=function(e) TRUE))
}
bc <- bray_curtis_dissimilarity
stopifnot(abs(bc(c(1,2), c(2,4)) - 1/3) < 1e-12)
stopifnot(abs(bc(c(2,4), c(1,2)) - 1/3) < 1e-12)
stopifnot(bc(c(0,0), c(0,0)) == 0)
stopifnot(bc(c(0,1), c(1,0)) == 1)
stopifnot(bc(c(1,2), c(1,2)) == 0)
stopifnot(abs(bc(c(1e308,1e308), c(1e308,0)) - 1/3) < 1e-12)
expect_error(bc(numeric(), numeric()))
expect_error(bc(1, c(1,2)))
expect_error(bc(-1, 1))
expect_error(bc(TRUE, 1))
expect_error(bc(NaN, 1))
cat("R Bray-Curtis dissimilarity tests passed\n")
