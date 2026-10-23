//! Scale-invariant cosine similarity for finite, nonzero vectors.

/// Return a similarity in [-1, 1] or an explicit validation error.
pub fn cosine_similarity(left: &[f64], right: &[f64]) -> Result<f64, &'static str> {
    if left.len() != right.len() || left.is_empty() {
        return Err("vectors must be nonempty and have equal lengths");
    }
    let mut left_max: f64 = 0.0;
    let mut right_max: f64 = 0.0;
    for (&a, &b) in left.iter().zip(right) {
        if !a.is_finite() || !b.is_finite() {
            return Err("components must be finite");
        }
        left_max = left_max.max(a.abs());
        right_max = right_max.max(b.abs());
    }
    if left_max == 0.0 || right_max == 0.0 {
        return Err("cosine similarity is undefined for zero vectors");
    }

    let (mut dot, mut left_sq, mut right_sq) = (0.0_f64, 0.0_f64, 0.0_f64);
    for (&a, &b) in left.iter().zip(right) {
        let x = a / left_max;
        let y = b / right_max;
        dot += x * y;
        left_sq += x * x;
        right_sq += y * y;
    }
    let value = dot / (left_sq.sqrt() * right_sq.sqrt());
    if !value.is_finite() {
        return Err("similarity is not finite");
    }
    Ok(value.clamp(-1.0, 1.0))
}

#[cfg(test)]
mod tests {
    use super::cosine_similarity;

    #[test]
    fn parallel_orthogonal_and_opposite() {
        assert!((cosine_similarity(&[1.0, 2.0], &[2.0, 4.0]).unwrap() - 1.0).abs() < 1e-12);
        assert!(cosine_similarity(&[1.0, 2.0], &[-2.0, 1.0]).unwrap().abs() < 1e-12);
        assert!((cosine_similarity(&[1.0, 2.0], &[-1.0, -2.0]).unwrap() + 1.0).abs() < 1e-12);
    }

    #[test]
    fn numeric_limits_and_invalid() {
        assert!((cosine_similarity(&[1e308, 1e308], &[1e-308, 1e-308]).unwrap() - 1.0).abs() < 1e-12);
        assert!(cosine_similarity(&[], &[]).is_err());
        assert!(cosine_similarity(&[1.0], &[]).is_err());
        assert!(cosine_similarity(&[0.0], &[1.0]).is_err());
        assert!(cosine_similarity(&[f64::NAN], &[1.0]).is_err());
        assert!(cosine_similarity(&[1.0], &[f64::INFINITY]).is_err());
    }
}
