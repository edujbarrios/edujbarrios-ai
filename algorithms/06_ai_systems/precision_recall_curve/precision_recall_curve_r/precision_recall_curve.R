# Exact binary precision–recall points at distinct score thresholds.
precision_recall_curve <- function(scores, labels) {
  if (!is.numeric(scores) || is.complex(scores) ||
      !is.numeric(labels) || is.complex(labels) ||
      length(scores) == 0L || length(scores) != length(labels) ||
      any(!is.finite(scores)) || any(scores < 0 | scores > 1) ||
      any(!is.finite(labels)) || any(!(labels %in% c(0, 1)))) {
    stop("nonempty aligned probability scores and binary labels required")
  }
  positives <- sum(labels)
  if (positives == 0) stop("at least one positive label required")
  n <- length(scores)
  indices <- order(-as.double(scores), seq_along(scores))
  sorted_scores <- as.double(scores[indices])
  sorted_labels <- as.double(labels[indices])
  points <- matrix(0, nrow = n, ncol = 3,
                   dimnames = list(NULL, c("threshold", "precision", "recall")))
  end <- 1L
  count <- 0L
  true_positives <- 0
  while (end <= n) {
    threshold <- sorted_scores[[end]]
    while (end <= n && sorted_scores[[end]] == threshold) {
      true_positives <- true_positives + sorted_labels[[end]]
      end <- end + 1L
    }
    count <- count + 1L
    points[count, ] <- c(threshold, true_positives / (end - 1L),
                         true_positives / positives)
  }
  points[seq_len(count), , drop = FALSE]
}
