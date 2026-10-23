//! Stable L2 normalization of nonzero finite vectors.

/// Return a unit vector with the same length or an error for invalid input.
pub fn vector_normalization(values: &[f64]) -> Result<Vec<f64>, &'static str> {
    if values.is_empty() {
        return Err("cannot normalize an empty vector");
    }
    let mut scale: f64 = 0.0;
    for &value in values {
        if !value.is_finite() {
            return Err("components must be finite");
        }
        scale = scale.max(value.abs());
    }
    if scale == 0.0 {
        return Err("cannot normalize a zero vector");
    }
    let squared: f64 = values.iter().map(|&x| (x / scale).powi(2)).sum();
    let norm = squared.sqrt();
    if !norm.is_finite() || norm == 0.0 {
        return Err("invalid norm");
    }
    Ok(values.iter().map(|&x| (x / scale) / norm).collect())
}

#[cfg(test)]
mod tests {
    use super::vector_normalization;

    #[test]
    fn typical_and_negative() {
        let x = vector_normalization(&[3.0, 4.0]).unwrap();
        assert!((x[0] - 0.6).abs() < 1e-12 && (x[1] - 0.8).abs() < 1e-12);
        let y = vector_normalization(&[-3.0, 4.0]).unwrap();
        assert!((y[0] + 0.6).abs() < 1e-12);
    }

    #[test]
    fn extremes() {
        assert_eq!(vector_normalization(&[1e-308]), Ok(vec![1.0]));
        let x = vector_normalization(&[1e308, 1e308]).unwrap();
        assert!((x[0].hypot(x[1]) - 1.0).abs() < 1e-12);
    }

    #[test]
    fn invalid_values() {
        assert!(vector_normalization(&[]).is_err());
        assert!(vector_normalization(&[0.0, 0.0]).is_err());
        assert!(vector_normalization(&[f64::NAN]).is_err());
        assert!(vector_normalization(&[f64::INFINITY]).is_err());
    }
}
