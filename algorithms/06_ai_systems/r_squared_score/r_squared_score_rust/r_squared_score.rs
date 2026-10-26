//! Coefficient of determination with finite constant-target convention.

/// Return a finite R-squared score for at least two paired f64 observations.
pub fn r_squared_score(actual: &[f64], predicted: &[f64]) -> Result<f64, &'static str> {
    if actual.len() < 2 || actual.len() != predicted.len() {
        return Err("equal lengths of at least two required");
    }
    let mut target_scale = 0.0_f64;
    let mut residual_scale = 0.0_f64;
    let (mut constant, mut perfect) = (true, true);
    for (&y, &prediction) in actual.iter().zip(predicted) {
        if !y.is_finite() || !prediction.is_finite() {
            return Err("finite observations required");
        }
        if y != actual[0] { constant = false; }
        if y != prediction { perfect = false; }
        target_scale = target_scale.max(y.abs());
        residual_scale = residual_scale.max(prediction.abs());
    }
    if constant { return Ok(if perfect { 1.0 } else { 0.0 }); }
    residual_scale = residual_scale.max(target_scale);
    let baseline = actual[0] / target_scale;
    let mut mean_offset = 0.0_f64;
    for &value in actual {
        mean_offset += (value / target_scale - baseline) / actual.len() as f64;
    }
    let (mut variation, mut squared_error) = (0.0_f64, 0.0_f64);
    for (&y, &prediction) in actual.iter().zip(predicted) {
        let deviation = (y / target_scale - baseline) - mean_offset;
        let residual = y / residual_scale - prediction / residual_scale;
        variation += deviation * deviation;
        squared_error += residual * residual;
    }
    if variation == 0.0 || !variation.is_finite() || !squared_error.is_finite() {
        return Err("unrepresentable variation or squared error");
    }
    let magnitude = residual_scale / target_scale;
    let score = 1.0 - squared_error / variation * magnitude * magnitude;
    if !score.is_finite() { return Err("unrepresentable R-squared"); }
    Ok(score)
}

#[cfg(test)]
mod tests {
    use super::r_squared_score as r2;
    #[test]
    fn reference() {
        assert_eq!(r2(&[1.0,2.0,3.0], &[1.0,2.0,3.0]).unwrap(), 1.0);
        assert!((r2(&[1.0,2.0,3.0], &[1.0,2.0,2.0]).unwrap() - 0.5).abs() < 1e-12);
        assert!((r2(&[1.0,2.0,3.0], &[0.0,0.0,0.0]).unwrap() + 6.0).abs() < 1e-12);
        assert_eq!(r2(&[4.0,4.0,4.0], &[4.0,4.0,4.0]).unwrap(), 1.0);
        assert_eq!(r2(&[4.0,4.0,4.0], &[4.0,4.0,0.0]).unwrap(), 0.0);
        assert!(r2(&[1e308,-1e308], &[0.0,0.0]).unwrap().abs() < 1e-12);
    }
    #[test]
    fn invalid() {
        assert!(r2(&[], &[]).is_err());
        assert!(r2(&[1.0], &[1.0]).is_err());
        assert!(r2(&[1.0,2.0], &[1.0]).is_err());
        assert!(r2(&[f64::NAN,2.0], &[1.0,2.0]).is_err());
        assert!(r2(&[f64::INFINITY,2.0], &[1.0,2.0]).is_err());
    }
}
