//! Median absolute regression error, including safe even-number midpoint.

/// Return median absolute residual for nonempty equal-length finite vectors.
pub fn median_absolute_error(actual: &[f64], predicted: &[f64]) -> Result<f64, &'static str> {
    if actual.is_empty() || actual.len() != predicted.len() {
        return Err("equal nonempty vector lengths required");
    }
    let mut errors = Vec::with_capacity(actual.len());
    for (&a, &b) in actual.iter().zip(predicted) {
        if !a.is_finite() || !b.is_finite() {
            return Err("finite observations required");
        }
        let error = (a - b).abs();
        if !error.is_finite() {
            return Err("absolute residual overflow");
        }
        errors.push(error);
    }
    errors.sort_unstable_by(|a, b| a.total_cmp(b));
    let mid = errors.len() / 2;
    if errors.len() % 2 == 1 {
        return Ok(errors[mid]);
    }
    let lower = errors[mid - 1];
    Ok(lower + (errors[mid] - lower) / 2.0)
}

#[cfg(test)]
mod tests {
    use super::median_absolute_error as medae;

    #[test]
    fn fixtures() {
        assert_eq!(medae(&[0.0,10.0,0.0], &[0.0,0.0,0.0]).unwrap(), 0.0);
        assert_eq!(medae(&[1.0,2.0,3.0,4.0], &[0.0,0.0,0.0,0.0]).unwrap(), 2.5);
        assert_eq!(medae(&[1.0,2.0], &[1.0,2.0]).unwrap(), 0.0);
        assert_eq!(medae(&[-1.0,-3.0,4.0], &[0.0,0.0,0.0]).unwrap(), 3.0);
        assert_eq!(medae(&[5.0], &[-5.0]).unwrap(), 10.0);
        assert_eq!(medae(&[1e308,1e308], &[0.0,0.0]).unwrap(), 1e308);
    }

    #[test]
    fn invalid() {
        assert!(medae(&[], &[]).is_err());
        assert!(medae(&[1.0], &[1.0,2.0]).is_err());
        assert!(medae(&[f64::NAN], &[1.0]).is_err());
        assert!(medae(&[f64::INFINITY], &[1.0]).is_err());
        assert!(medae(&[1e308], &[-1e308]).is_err());
    }
}
