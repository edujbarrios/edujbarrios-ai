# Dense affine transform + ReLU, with one output neuron per weight row.
dense_forward <- function(input, weights, bias) {
  if (!is.numeric(input) || is.complex(input) ||
      length(input) < 1L || any(!is.finite(input)) ||
      !is.matrix(weights) || !is.numeric(weights) ||
      is.complex(weights) || nrow(weights) < 1L ||
      ncol(weights) != length(input) || any(!is.finite(weights)) ||
      !is.numeric(bias) || is.complex(bias) ||
      length(bias) != nrow(weights) || any(!is.finite(bias))) {
    stop("nonempty finite input, rectangular weights and matching biases required")
  }
  output <- numeric(nrow(weights))
  for (j in seq_len(nrow(weights))) {
    total <- 0.0
    for (i in seq_along(input)) {
      product <- as.double(weights[j,i]) * as.double(input[[i]])
      if (!is.finite(product)) stop("multiplication overflow")
      total <- total + product
      if (!is.finite(total)) stop("sum overflow")
    }
    z <- total + as.double(bias[[j]])
    if (!is.finite(z)) stop("bias addition overflow")
    output[[j]] <- max(0.0,z)
  }
  output
}
