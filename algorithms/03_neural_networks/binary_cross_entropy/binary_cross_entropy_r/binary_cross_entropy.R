binary_cross_entropy <- function(labels, probabilities) {
 if (!is.numeric(labels)||!is.numeric(probabilities)||is.complex(labels)||is.complex(probabilities)||
 length(labels)==0L||length(labels)!=length(probabilities)||any(!is.finite(labels))||
 any(!is.finite(probabilities))||any(!(labels %in% c(0,1)))||
 any(probabilities<0|probabilities>1)) stop("invalid labels or probabilities")
 terms <- ifelse(labels==1,-log(probabilities),-log1p(-probabilities))
 if(any(!is.finite(terms))) stop("infinite loss")
 result <- sum(terms)/length(labels)
 if(!is.finite(result)) stop("overflow")
 result
}
