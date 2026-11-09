# Exact ASCII longest palindromic substring, earliest-start tie breaking.
longest_palindromic_substring <- function(text) {
  if (!is.character(text) || length(text)!=1L || is.na(text)) {
    stop("expected one printable ASCII string")
  }
  chars <- utf8ToInt(enc2utf8(text))
  if (length(chars)>512L || any(chars<32L) || any(chars>126L)) {
    stop("expected printable ASCII string at most 512 characters")
  }
  n <- length(chars)
  best_length <- 0L
  best_start <- 0L
  for (center in seq_len(n)) {
    for (offset in 0:1) {
      left <- as.integer(center)
      right <- as.integer(center+offset)
      while (left>=1L && right<=n && chars[[left]]==chars[[right]]) {
        length <- as.integer(right-left+1L)
        start <- as.integer(left-1L)
        if (length>best_length || (length==best_length && start<best_start)) {
          best_length <- length
          best_start <- start
        }
        left <- left-1L
        right <- right+1L
      }
    }
  }
  list(length=best_length,start=best_start)
}
