# Intersection over union of two axis-aligned bounding boxes.
bounding_box_iou <- function(a, b) {
  valid <- function(box) {
    is.numeric(box) && !is.complex(box) && length(box) == 4L &&
      all(is.finite(box)) && all(abs(box) <= 1000000) &&
      box[[1L]] < box[[3L]] && box[[2L]] < box[[4L]]
  }
  if (!valid(a) || !valid(b)) stop("invalid box coordinates")
  width <- max(0, min(a[[3L]], b[[3L]]) - max(a[[1L]], b[[1L]]))
  height <- max(0, min(a[[4L]], b[[4L]]) - max(a[[2L]], b[[2L]]))
  intersection <- width * height
  area_a <- (a[[3L]] - a[[1L]]) * (a[[4L]] - a[[2L]])
  area_b <- (b[[3L]] - b[[1L]]) * (b[[4L]] - b[[2L]])
  intersection / (area_a + area_b - intersection)
}
