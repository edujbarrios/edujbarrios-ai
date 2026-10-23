//! One-predictor ordinary least squares via centered sums.

/// Return (slope, intercept), rejecting degenerate and unrepresentable inputs.
pub fn linear_regression(x: &[f64], y: &[f64]) -> Result<(f64, f64), &'static str> {
    if x.len() != y.len() || x.len() < 2 {
        return Err("at least two paired observations are required");
    }
    let mut mean_x: f64 = 0.0;
    let mut mean_y: f64 = 0.0;
    for (i, (&a, &b)) in x.iter().zip(y).enumerate() {
        if !a.is_finite() || !b.is_finite() {
            return Err("observations must be finite");
        }
        let dx = a - mean_x;
        let dy = b - mean_y;
        if !dx.is_finite() || !dy.is_finite() {
            return Err("mean overflow");
        }
        mean_x += dx / (i + 1) as f64;
        mean_y += dy / (i + 1) as f64;
        if !mean_x.is_finite() || !mean_y.is_finite() {
            return Err("mean overflow");
        }
    }
    let (mut xx, mut xy) = (0.0_f64, 0.0_f64);
    for (&a, &b) in x.iter().zip(y) {
        let dx = a - mean_x;
        let dy = b - mean_y;
        let squared = dx * dx;
        let product = dx * dy;
        if !dx.is_finite() || !dy.is_finite() ||
           !squared.is_finite() || !product.is_finite() {
            return Err("variance/covariance overflow");
        }
        xx += squared;
        xy += product;
        if !xx.is_finite() || !xy.is_finite() {
            return Err("variance/covariance overflow");
        }
    }
    if xx == 0.0 {
        return Err("x must not be constant");
    }
    let slope = xy / xx;
    let intercept = mean_y - slope * mean_x;
    if !slope.is_finite() || !intercept.is_finite() {
        return Err("regression overflow");
    }
    Ok((slope, intercept))
}

#[cfg(test)]
mod tests {
    use super::linear_regression;

    #[test]
    fn exact_and_horizontal_lines() {
        assert_eq!(linear_regression(&[0.,1.,2.,3.], &[1.,3.,5.,7.]), Ok((2.,1.)));
        assert_eq!(linear_regression(&[0.,1.,2.,3.], &[3.,3.,3.,3.]), Ok((0.,3.)));
    }
    #[test]
    fn invalid() {
        assert!(linear_regression(&[], &[]).is_err());
        assert!(linear_regression(&[1.], &[1.]).is_err());
        assert!(linear_regression(&[2.,2.], &[1.,3.]).is_err());
        assert!(linear_regression(&[f64::NAN,1.], &[1.,2.]).is_err());
        assert!(linear_regression(&[1e308,-1e308], &[1e308,-1e308]).is_err());
    }
}
