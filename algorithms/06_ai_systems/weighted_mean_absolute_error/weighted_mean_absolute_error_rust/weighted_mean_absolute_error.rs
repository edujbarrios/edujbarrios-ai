//! Normalized weighted mean absolute error for bounded finite vectors.
/// Return weighted MAE, or an error for invalid inputs.
pub fn weighted_mean_absolute_error(targets: &[f64], predictions: &[f64],
                                    weights: &[f64]) -> Result<f64, &'static str> {
    let n = targets.len();
    if n == 0 || n > 4096 || predictions.len() != n || weights.len() != n {
        return Err("vectors must be equal nonempty length <= 4096");
    }
    let mut total = 0.0;
    let mut weighted = 0.0;
    for ((&y, &p), &w) in targets.iter().zip(predictions).zip(weights) {
        if !y.is_finite() || !p.is_finite() || !w.is_finite() ||
           y.abs() > 1e6 || p.abs() > 1e6 || !(0.0..=1e6).contains(&w) {
            return Err("invalid targets, predictions or weights");
        }
        total += w;
        weighted += w * (y - p).abs();
    }
    if total <= 0.0 { return Err("weights must sum to a positive value"); }
    Ok(weighted / total)
}
#[cfg(test)]
mod tests {
    use super::weighted_mean_absolute_error as wmae;
    #[test] fn fixtures() {
        assert_eq!(wmae(&[1.,3.],&[2.,1.],&[1.,3.]),Ok(1.75));
        assert_eq!(wmae(&[1.,3.],&[1.,3.],&[1.,3.]),Ok(0.0));
        assert_eq!(wmae(&[1.,3.],&[2.,1.],&[0.,2.]),Ok(2.0));
        assert_eq!(wmae(&[1.],&[2.],&[1.]),Ok(1.0));
    }
    #[test] fn invalid() {
        assert!(wmae(&[],&[],&[]).is_err());
        assert!(wmae(&[1.],&[2.],&[]).is_err());
        assert!(wmae(&[1.],&[2.],&[0.]).is_err());
        assert!(wmae(&[1.],&[2.],&[-1.]).is_err());
        assert!(wmae(&[f64::NAN],&[2.],&[1.]).is_err());
        assert!(wmae(&[1.],&[2.],&[f64::INFINITY]).is_err());
        assert!(wmae(&[1000001.],&[2.],&[1.]).is_err());
    }
}
