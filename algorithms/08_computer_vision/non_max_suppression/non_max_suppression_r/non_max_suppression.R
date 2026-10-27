# Greedy IoU-based detection suppression, zero-based output indices.
non_max_suppression <- function(boxes, threshold) {
  if (!is.matrix(boxes) || !is.numeric(boxes) || is.complex(boxes) ||
      ncol(boxes)!=5L || nrow(boxes)>256L || any(!is.finite(boxes)) ||
      any(abs(boxes[,1:4,drop=FALSE])>1000000) ||
      any(boxes[,3]<=boxes[,1]) || any(boxes[,4]<=boxes[,2]) ||
      any(boxes[,5]<0) || any(boxes[,5]>1) ||
      !is.numeric(threshold) || is.complex(threshold) ||
      length(threshold)!=1L || !is.finite(threshold) ||
      threshold<0 || threshold>1) stop("invalid boxes or suppression threshold")
  n <- nrow(boxes)
  if (n==0L) return(integer())
  area <- function(box) (box[[3]]-box[[1]])*(box[[4]]-box[[2]])
  iou <- function(a,b) {
    width <- max(0,min(a[[3]],b[[3]])-max(a[[1]],b[[1]]))
    height <- max(0,min(a[[4]],b[[4]])-max(a[[2]],b[[2]]))
    intersection <- width*height
    intersection/(area(a)+area(b)-intersection)
  }
  order_indices <- order(-boxes[,5L],seq_len(n))
  kept <- integer()
  while (length(order_indices)>0L) {
    selected <- order_indices[[1L]]
    kept <- c(kept,selected-1L)
    if (length(order_indices)==1L) break
    rest <- order_indices[-1L]
    order_indices <- rest[vapply(rest,function(i)
      iou(boxes[selected,],boxes[i,])<=threshold,logical(1L))]
  }
  kept
}
