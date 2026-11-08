//! Raw median absolute deviation of finite real observations.

fn median_sorted(sorted: &[f64]) -> f64 {
    let n = sorted.len();
    if n % 2 != 0 {
        return sorted[n / 2];
    }
    let (left, right) = (sorted[n / 2 - 1], sorted[n / 2]);
    if left <= 0.0 && right >= 0.0 {
        left / 2.0 + right / 2.0
    } else {
        left + (right - left) / 2.0
    }
}

/// Unscaled MAD; reject invalid length, nonfinite values or overflow.
pub fn median_absolute_deviation(values: &[f64]) -> Result<f64, &'static str> {
    if !(1..=512).contains(&values.len()) || values.iter().any(|x| !x.is_finite()) {
        return Err("expected 1..512 finite observations");
    }
    let mut sorted = values.to_vec();
    sorted.sort_by(f64::total_cmp);
    let center = median_sorted(&sorted);
    if !center.is_finite() {
        return Err("nonfinite center");
    }
    let mut deviations: Vec<f64> = values.iter().map(|x| (x - center).abs()).collect();
    if deviations.iter().any(|x| !x.is_finite()) {
        return Err("nonfinite deviation");
    }
    deviations.sort_by(f64::total_cmp);
    let result = median_sorted(&deviations);
    if !result.is_finite() || result < 0.0 {
        return Err("nonfinite result");
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::median_absolute_deviation as mad;
    #[test]
    fn examples() {
        assert_eq!(mad(&[1., 1., 2., 2., 4., 6., 9.]), Ok(1.));
        assert_eq!(mad(&[1., 2., 3., 10.]), Ok(1.));
        assert_eq!(mad(&[10., 3., 2., 1.]), Ok(1.));
        assert_eq!(mad(&[5., 5., 5.]), Ok(0.));
        assert_eq!(mad(&[42.]), Ok(0.));
        assert_eq!(mad(&[-3., -1., 2.]), Ok(2.));
        assert_eq!(mad(&[7.; 512]), Ok(0.));
        assert_eq!(mad(&[-1e308, 1e308]), Ok(1e308));
    }
    #[test]
    fn invalid() {
        assert!(mad(&[]).is_err());
        assert!(mad(&[1.; 513]).is_err());
        assert!(mad(&[f64::INFINITY]).is_err());
        assert!(mad(&[f64::NAN]).is_err());
        assert!(mad(&[-1e308, -1e308, 1e308]).is_err());
    }
}
