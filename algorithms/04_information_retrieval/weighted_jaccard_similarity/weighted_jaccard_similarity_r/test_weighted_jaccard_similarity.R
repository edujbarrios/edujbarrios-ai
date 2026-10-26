args <- grep("^--file=", commandArgs(FALSE), value=TRUE)
folder <- dirname(normalizePath(sub("^--file=", "", args[[1L]])))
source(file.path(folder, "weighted_jaccard_similarity.R"))
expect_error <- function(expr) {
  stopifnot(tryCatch({force(expr); FALSE}, error=function(e) TRUE))
}
wj <- weighted_jaccard_similarity
stopifnot(abs(wj(c(1,2,0), c(1,1,3)) - 1/3) < 1e-12)
stopifnot(wj(c(0,0), c(0,0)) == 1)
stopifnot(wj(c(1,0), c(0,1)) == 0)
stopifnot(wj(c(1,2), c(1,2)) == 1)
stopifnot(abs(wj(c(0.2,0.8), c(0.5,0.5)) - 7/13) < 1e-12)
stopifnot(wj(c(1e308,1e308), c(1e308,0)) == 0.5)
expect_error(wj(numeric(), numeric()))
expect_error(wj(1, c(1,2)))
expect_error(wj(-1, 0))
expect_error(wj(TRUE, 1))
expect_error(wj(NaN, 0))
cat("R weighted Jaccard tests passed\n")
