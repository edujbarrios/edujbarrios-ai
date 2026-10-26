//! Chebyshev (L-infinity) distance over finite real vectors.

pub fn chebyshev_distance(left: &[f64], right: &[f64]) -> Result<f64, &'static str> {
    if left.len() != right.len() {
        return Err("vectors must have equal lengths");
    }
    let mut maximum = 0.0_f64;
    for (&a, &b) in left.iter().zip(right) {
        if !a.is_finite() || !b.is_finite() {
            return Err("non-finite coordinate");
        }
        let difference = a - b;
        if !difference.is_finite() {
            return Err("distance overflow");
        }
        maximum = maximum.max(difference.abs());
    }
    Ok(maximum)
}

#[cfg(test)]
mod tests {
    use super::chebyshev_distance as distance;

    #[test]
    fn examples() {
        assert_eq!(distance(&[1.0, 2.0, -1.0], &[4.0, -2.0, 1.0]), Ok(4.0));
        assert_eq!(distance(&[4.0, -2.0, 1.0], &[1.0, 2.0, -1.0]), Ok(4.0));
        assert_eq!(distance(&[1.0], &[1.0]), Ok(0.0));
        assert_eq!(distance(&[], &[]), Ok(0.0));
        assert_eq!(distance(&[1e308], &[0.0]), Ok(1e308));
    }

    #[test]
    fn rejects_invalid_data() {
        assert!(distance(&[1.0], &[]).is_err());
        assert!(distance(&[f64::NAN], &[0.0]).is_err());
        assert!(distance(&[f64::INFINITY], &[0.0]).is_err());
        assert!(distance(&[1e308], &[-1e308]).is_err());
    }
}
