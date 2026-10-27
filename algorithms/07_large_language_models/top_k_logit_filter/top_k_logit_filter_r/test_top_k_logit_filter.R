file_arg <- grep("^--file=",commandArgs(FALSE),value=TRUE)
if (length(file_arg)==0L) stop("run with Rscript")
folder <- dirname(normalizePath(sub("^--file=","",file_arg[[1L]])))
source(file.path(folder,"top_k_logit_filter.R"))
expect_error <- function(expr) {
  failed <- tryCatch({force(expr);FALSE},error=function(e) TRUE)
  stopifnot(failed)
}
stopifnot(identical(top_k_logit_filter(c(2,5,5,-1),2),
                    c(FALSE,TRUE,TRUE,FALSE)))
stopifnot(identical(top_k_logit_filter(c(2,5,5,-1),1),
                    c(FALSE,TRUE,FALSE,FALSE)))
stopifnot(identical(top_k_logit_filter(c(2,5,5,-1),4),rep(TRUE,4)))
stopifnot(identical(top_k_logit_filter(c(-2,-1,-3,-1),2),
                    c(FALSE,TRUE,FALSE,TRUE)))
stopifnot(identical(top_k_logit_filter(-3,1),TRUE))
expect_error(top_k_logit_filter(numeric(),1))
expect_error(top_k_logit_filter(rep(1,4097),1))
expect_error(top_k_logit_filter(c(1),0))
expect_error(top_k_logit_filter(c(1),2))
expect_error(top_k_logit_filter(c(1),1.5))
expect_error(top_k_logit_filter(c(NaN),1))
expect_error(top_k_logit_filter(c(Inf),1))
expect_error(top_k_logit_filter(c(1001),1))
cat("R top-k filtering tests passed\n")
