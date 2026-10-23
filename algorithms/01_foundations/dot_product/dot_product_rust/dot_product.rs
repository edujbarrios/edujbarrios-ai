//! A finite real-valued dot product with explicit errors.

/// Compute a dot product. Empty vectors return 0.0.
/// Rejects dimension mismatches, non-finite values, and overflow.
pub fn dot_product(left: &[f64], right: &[f64]) -> Result<f64, &'static str> {
    if left.len() != right.len() {
        return Err("vectors must have equal lengths");
    }
    let mut total = 0.0;
    for (&a, &b) in left.iter().zip(right) {
        if !a.is_finite() || !b.is_finite() {
            return Err("values must be finite");
        }
        let product = a * b;
        if !product.is_finite() {
            return Err("dot product overflow");
        }
        total += product;
        if !total.is_finite() {
            return Err("dot product overflow");
        }
    }
    Ok(total)
}

#[cfg(test)]
mod tests {
    use super::dot_product;

    #[test]
    fn positive_and_negative() {
        assert_eq!(dot_product(&[1.0, 2.0, 3.0], &[4.0, 5.0, 6.0]), Ok(32.0));
        assert_eq!(dot_product(&[-1.0, 2.0, -3.0], &[4.0, -5.0, 6.0]), Ok(-32.0));
        assert_eq!(dot_product(&[0.5, -1.5], &[2.0, 4.0]), Ok(-5.0));
    }

    #[test]
    fn empty_is_zero() {
        assert_eq!(dot_product(&[], &[]), Ok(0.0));
    }

    #[test]
    fn invalid_dimensions_and_values() {
        assert!(dot_product(&[1.0], &[]).is_err());
        assert!(dot_product(&[f64::NAN], &[1.0]).is_err());
        assert!(dot_product(&[f64::INFINITY], &[1.0]).is_err());
        assert!(dot_product(&[f64::MAX], &[f64::MAX]).is_err());
    }
}
