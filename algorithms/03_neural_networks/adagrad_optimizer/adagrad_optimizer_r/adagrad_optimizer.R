# One AdaGrad optimizer step with explicit accumulated squared gradients.
adagrad_optimizer <- function(parameters, gradients, accumulator,
                              learning_rate, epsilon) {
  if (!is.numeric(parameters) || is.complex(parameters) ||
      !is.numeric(gradients) || is.complex(gradients) ||
      !is.numeric(accumulator) || is.complex(accumulator) ||
      length(parameters) != length(gradients) ||
      length(parameters) != length(accumulator) ||
      length(learning_rate) != 1L || !is.numeric(learning_rate) ||
      is.complex(learning_rate) || !is.finite(learning_rate) || learning_rate <= 0 ||
      length(epsilon) != 1L || !is.numeric(epsilon) ||
      is.complex(epsilon) || !is.finite(epsilon) || epsilon <= 0 ||
      any(!is.finite(parameters)) || any(!is.finite(gradients)) ||
      any(!is.finite(accumulator)) || any(accumulator < 0)) {
    stop("invalid aligned finite inputs or hyperparameters")
  }
  updated_h <- as.double(accumulator) + as.double(gradients)^2
  denominator <- sqrt(updated_h) + epsilon
  step <- learning_rate * as.double(gradients) / denominator
  updated_w <- as.double(parameters) - step
  if (any(!is.finite(updated_h)) || any(!is.finite(denominator)) ||
      any(!is.finite(step)) || any(!is.finite(updated_w))) {
    stop("nonfinite AdaGrad intermediate")
  }
  list(parameters = updated_w, accumulator = updated_h)
}
