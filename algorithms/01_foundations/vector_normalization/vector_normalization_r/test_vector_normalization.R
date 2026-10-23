file_arg <- grep("^--file=", commandArgs(FALSE), value = TRUE)
if (length(file_arg) == 0L) stop("run with Rscript")
folder <- dirname(normalizePath(sub("^--file=", "", file_arg[[1L]])))
source(file.path(folder, "vector_normalization.R"))

expect_error <- function(expr) {
  failed <- tryCatch({ force(expr); FALSE }, error = function(e) TRUE)
  stopifnot(failed)
}
stopifnot(max(abs(vector_normalization(c(3, 4)) - c(0.6, 0.8))) < 1e-12)
stopifnot(max(abs(vector_normalization(c(-3, 4)) - c(-0.6, 0.8))) < 1e-12)
stopifnot(identical(vector_normalization(c(1e-308)), 1.0))
stopifnot(abs(sqrt(sum(vector_normalization(c(1e308, 1e308))^2)) - 1) < 1e-12)
expect_error(vector_normalization(numeric()))
expect_error(vector_normalization(c(0, 0)))
expect_error(vector_normalization(c(NaN)))
expect_error(vector_normalization(c(Inf)))
cat("R vector normalization tests passed\n")
