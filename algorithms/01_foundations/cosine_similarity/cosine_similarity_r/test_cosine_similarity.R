file_arg <- grep("^--file=", commandArgs(FALSE), value = TRUE)
if (length(file_arg) == 0L) stop("run with Rscript")
folder <- dirname(normalizePath(sub("^--file=", "", file_arg[[1L]])))
source(file.path(folder, "cosine_similarity.R"))

expect_error <- function(expr) {
  did_error <- tryCatch({ force(expr); FALSE }, error = function(e) TRUE)
  stopifnot(did_error)
}
stopifnot(abs(cosine_similarity(c(1, 2), c(2, 4)) - 1) < 1e-12)
stopifnot(abs(cosine_similarity(c(1, 2), c(-1, -2)) + 1) < 1e-12)
stopifnot(abs(cosine_similarity(c(1, 2), c(-2, 1))) < 1e-12)
stopifnot(abs(cosine_similarity(c(1e308, 1e308), c(1e-308, 1e-308)) - 1) < 1e-12)
expect_error(cosine_similarity(numeric(), numeric()))
expect_error(cosine_similarity(c(1), numeric()))
expect_error(cosine_similarity(c(0), c(1)))
expect_error(cosine_similarity(c(NaN), c(1)))
expect_error(cosine_similarity(c(1), c(Inf)))
cat("R cosine similarity tests passed\n")
