# Linear warmup to peak rate, then cosine decay to the minimum rate.
warmup_cosine_learning_rate <- function(peak_rate, minimum_rate,
                                        warmup_steps, total_steps, step) {
  valid_rate <- function(x) {
    is.numeric(x) && !is.complex(x) && length(x) == 1L && is.finite(x)
  }
  valid_step <- function(x) {
    valid_rate(x) && x >= 0 && x == floor(x)
  }
  if (!valid_rate(peak_rate) || !valid_rate(minimum_rate) ||
      minimum_rate < 0 || minimum_rate > peak_rate || peak_rate > 1000000 ||
      !valid_step(warmup_steps) || !valid_step(total_steps) || !valid_step(step) ||
      warmup_steps < 1 || warmup_steps >= total_steps || total_steps > 100000 ||
      step > total_steps) stop("invalid learning rates or step bounds")
  if (step <= warmup_steps) return(as.double(peak_rate * (step / warmup_steps)))
  if (step == total_steps) return(as.double(minimum_rate))
  progress <- (step - warmup_steps) / (total_steps - warmup_steps)
  as.double(minimum_rate + (peak_rate - minimum_rate) * 0.5 * (1 + cos(pi * progress)))
}
