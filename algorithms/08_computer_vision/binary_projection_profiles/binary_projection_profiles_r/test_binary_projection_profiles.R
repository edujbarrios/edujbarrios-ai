args <- grep("^--file=",commandArgs(FALSE),value=TRUE)
folder <- dirname(normalizePath(sub("^--file=","",args[[1L]])))
source(file.path(folder,"binary_projection_profiles.R"))
expect_error <- function(expr) stopifnot(tryCatch({force(expr);FALSE},error=function(e)TRUE))
check <- function(p,h,w,want) {
  stopifnot(identical(binary_projection_profiles(p,h,w),as.integer(want)))
}
check(c(1,0,1,0,1,1),2,3,c(2,2,1,1,2))
check(c(1,0,1,1),1,4,c(3,1,0,1,1))
check(c(1,0,1,1),4,1,c(1,0,1,1,3))
check(rep(0,4),2,2,rep(0,4))
check(rep(1,4),2,2,rep(2,4))
check(0,1,1,c(0,0))
check(1,1,1,c(1,1))
check(c(1,0,0,1),2,2,c(1,1,1,1))
check(rep(1,4096),64,64,rep(64,128))
check(rep(0,4096),64,64,rep(0,128))
checker <- integer(4096)
for (r in 0:63) for (c in 0:63) checker[[r*64L+c+1L]] <- (r+c)%%2L
check(checker,64,64,rep(32,128))
mixed <- integer(4096)
mixed[[1L]] <- 1L
mixed[[4096L]] <- 1L
profile <- binary_projection_profiles(mixed,64,64)
stopifnot(profile[[1L]]==1L,profile[[64L]]==1L,
          profile[[65L]]==1L,profile[[128L]]==1L,
          sum(profile[1:64])==2,sum(profile[65:128])==2)
expect_error(binary_projection_profiles(0,0,1))
expect_error(binary_projection_profiles(0,1,0))
expect_error(binary_projection_profiles(0,65,1))
expect_error(binary_projection_profiles(0,1,65))
expect_error(binary_projection_profiles(TRUE,1,1))
expect_error(binary_projection_profiles(0,TRUE,1))
expect_error(binary_projection_profiles(c(0,2),1,2))
expect_error(binary_projection_profiles(0.5,1,1))
expect_error(binary_projection_profiles(c(0,NA_real_),1,2))
expect_error(binary_projection_profiles(matrix(1:4,2,2),2,2))
cat("R projection profiles tests passed\n")
