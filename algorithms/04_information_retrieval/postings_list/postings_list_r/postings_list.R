# Build postings for one signed-32-bit integer term across documents.
postings_list <- function(documents, target) {
  valid_ids <- function(x) {
    is.numeric(x) && !is.complex(x) && all(is.finite(x)) &&
      all(x == floor(x)) && all(x >= -2147483648) &&
      all(x <= 2147483647)
  }
  if (!is.list(documents) || !valid_ids(target) || length(target) != 1L) {
    stop("documents must be a list and target a signed 32-bit integer")
  }

  ids <- integer()
  frequencies <- integer()
  for (i in seq_along(documents)) {
    document <- documents[[i]]
    if (!valid_ids(document)) {
      stop("document tokens must be signed 32-bit integers")
    }
    frequency <- sum(as.double(document) == target)
    if (frequency > 0) {
      ids <- c(ids, as.integer(i - 1L))
      frequencies <- c(frequencies, as.integer(frequency))
    }
  }
  list(doc_ids = ids, frequencies = frequencies)
}
