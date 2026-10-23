# Sign hash of finite nonzero vectors and nonzero row-wise hyperplane normals.
random_hyperplane_hash <- function(vector, normals) {
  if (!is.numeric(vector) || is.complex(vector) ||
      length(vector) == 0L || any(!is.finite(vector)) ||
      !any(vector != 0) || !is.matrix(normals) ||
      !is.numeric(normals) || is.complex(normals) ||
      nrow(normals) < 1L || nrow(normals) > 30L ||
      ncol(normals) != length(vector) || any(!is.finite(normals))) {
    stop("invalid vector, normals or dimensions")
  }
  signature <- 0L
  for (j in seq_len(nrow(normals))) {
    row <- as.double(normals[j, ])
    if (!any(row != 0)) stop("zero normal")
    dot <- 0.0
    for (i in seq_along(vector)) {
      product <- as.double(vector[[i]]) * row[[i]]
      if (!is.finite(product)) stop("projection product overflow")
      dot <- dot + product
      if (!is.finite(dot)) stop("projection sum overflow")
    }
    if (dot >= 0) {
      signature <- bitwOr(signature, bitwShiftL(1L, j-1L))
    }
  }
  as.integer(signature)
}
