//! Stable L2 distance of finite real vectors.

/// Compute a Euclidean distance using a numerically stable hypotenuse fold.
pub fn euclidean_distance(left: &[f64], right: &[f64]) -> Result<f64, &'static str> {
    if left.len() != right.len() {
        return Err("vectors must have equal lengths");
    }

    let mut distance = 0.0;
    for (&a, &b) in left.iter().zip(right) {
        if !a.is_finite() || !b.is_finite() {
            return Err("values must be finite");
        }
        let difference = a - b;
        if !difference.is_finite() {
            return Err("distance overflow");
        }
        distance = distance.hypot(difference);
        if !distance.is_finite() {
            return Err("distance overflow");
        }
    }
    Ok(distance)
}

#[cfg(test)]
mod tests {
    use super::euclidean_distance;

    #[test]
    fn expected_values() {
        assert_eq!(euclidean_distance(&[0.0, 0.0], &[3.0, 4.0]), Ok(5.0));
        assert_eq!(euclidean_distance(&[1.0, 2.0], &[1.0, 2.0]), Ok(0.0));
        assert_eq!(euclidean_distance(&[], &[]), Ok(0.0));
    }

    #[test]
    fn large_but_finite_distance() {
        let result = euclidean_distance(&[1e154], &[0.0]).unwrap();
        assert!((result / 1e154 - 1.0).abs() < 1e-12);
    }

    #[test]
    fn invalid_inputs() {
        assert!(euclidean_distance(&[1.0], &[]).is_err());
        assert!(euclidean_distance(&[f64::NAN], &[1.0]).is_err());
        assert!(euclidean_distance(&[f64::INFINITY], &[1.0]).is_err());
        assert!(euclidean_distance(&[1e308], &[-1e308]).is_err());
    }
}
