//! Online mean and population variance using Welford's recurrence.

/// Return (mean, population_variance) for nonempty finite observations.
pub fn mean_variance(values: &[f64]) -> Result<(f64, f64), &'static str> {
    if values.is_empty() {
        return Err("observations must be nonempty");
    }
    let mut mean: f64 = 0.0;
    let mut m2: f64 = 0.0;
    for (index, &x) in values.iter().enumerate() {
        if !x.is_finite() {
            return Err("observations must be finite");
        }
        let delta = x - mean;
        if !delta.is_finite() {
            return Err("intermediate overflow");
        }
        mean += delta / (index + 1) as f64;
        let delta2 = x - mean;
        let increment = delta * delta2;
        if !mean.is_finite() || !delta2.is_finite() || !increment.is_finite() {
            return Err("intermediate overflow");
        }
        m2 += increment;
        if !m2.is_finite() {
            return Err("variance overflow");
        }
    }
    let variance = m2 / values.len() as f64;
    if !variance.is_finite() || variance < 0.0 {
        return Err("variance is not finite");
    }
    Ok((mean, variance))
}

#[cfg(test)]
mod tests {
    use super::mean_variance;

    #[test]
    fn known_result_and_constant() {
        assert_eq!(mean_variance(&[2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0]), Ok((5.0, 4.0)));
        assert_eq!(mean_variance(&[7.0, 7.0, 7.0]), Ok((7.0, 0.0)));
        assert_eq!(mean_variance(&[3.0]), Ok((3.0, 0.0)));
    }

    #[test]
    fn negative() {
        let (mean, variance) = mean_variance(&[-2.0, 0.0, 2.0]).unwrap();
        assert!(mean.abs() < 1e-12);
        assert!((variance - 8.0 / 3.0).abs() < 1e-12);
    }

    #[test]
    fn invalid_input() {
        assert!(mean_variance(&[]).is_err());
        assert!(mean_variance(&[f64::NAN]).is_err());
        assert!(mean_variance(&[f64::INFINITY]).is_err());
        assert!(mean_variance(&[1e308, -1e308]).is_err());
    }
}
