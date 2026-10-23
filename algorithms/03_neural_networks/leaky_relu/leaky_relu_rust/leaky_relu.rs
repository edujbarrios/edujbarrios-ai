//! Elementwise Leaky ReLU forward activation.

pub fn leaky_relu(values: &[f64], alpha: f64) -> Result<Vec<f64>, &'static str> {
    if !alpha.is_finite() || alpha <= 0.0 || alpha >= 1.0 {
        return Err("alpha must be finite and in (0, 1)");
    }
    if !values.iter().all(|v| v.is_finite()) {
        return Err("components must be finite");
    }
    Ok(values.iter().map(|&x| if x < 0.0 { alpha * x } else { x }).collect())
}

#[cfg(test)]
mod tests {
    use super::leaky_relu;

    #[test]
    fn values_and_empty() {
        let result = leaky_relu(&[-2.0, 0.0, 3.0], 0.1).unwrap();
        assert!((result[0] + 0.2).abs() < 1e-12);
        assert_eq!(&result[1..], &[0.0, 3.0]);
        assert_eq!(leaky_relu(&[], 0.1), Ok(vec![]));
    }

    #[test]
    fn invalid() {
        assert!(leaky_relu(&[1.0], 0.0).is_err());
        assert!(leaky_relu(&[1.0], 1.0).is_err());
        assert!(leaky_relu(&[f64::NAN], 0.1).is_err());
        assert!(leaky_relu(&[1.0], f64::INFINITY).is_err());
    }
}
