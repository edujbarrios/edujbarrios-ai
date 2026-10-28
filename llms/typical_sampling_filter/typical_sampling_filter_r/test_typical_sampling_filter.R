args <- grep("^--file=",commandArgs(FALSE),value=TRUE)
if (length(args)==0L) stop("run with Rscript")
source(file.path(dirname(normalizePath(sub("^--file=","",args[[1L]]))),
                 "typical_sampling_filter.R"))
bad <- function(expr) stopifnot(tryCatch({force(expr);FALSE},error=function(e) TRUE))
stopifnot(identical(typical_sampling_filter(c(0,0,0,0),0.6),
                    c(TRUE,TRUE,TRUE,FALSE)))
stopifnot(identical(typical_sampling_filter(c(2,1,0),0.8),
                    c(TRUE,TRUE,FALSE)))
stopifnot(identical(typical_sampling_filter(rep(0,4),1),rep(TRUE,4)))
stopifnot(identical(typical_sampling_filter(c(-1000,1000,0),0.8),
                    c(FALSE,TRUE,FALSE)))
stopifnot(identical(typical_sampling_filter(5,0.5),TRUE))
bad(typical_sampling_filter(numeric(),0.5))
bad(typical_sampling_filter(rep(0,4097),0.5))
bad(typical_sampling_filter(0,0))
bad(typical_sampling_filter(0,-1))
bad(typical_sampling_filter(0,1.1))
bad(typical_sampling_filter(0,NaN))
bad(typical_sampling_filter(Inf,0.5))
bad(typical_sampling_filter(1001,0.5))
cat("R locally typical filtering tests passed\n")
