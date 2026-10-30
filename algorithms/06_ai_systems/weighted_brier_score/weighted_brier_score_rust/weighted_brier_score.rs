//! Sample-weighted binary Brier score for probabilistic forecasts.
/// Return the weighted mean squared probability error, or Err on invalid input.
pub fn weighted_brier_score(targets: &[f64], probabilities: &[f64],
                            weights: &[f64]) -> Result<f64, &'static str> {
    let n = targets.len();
    if n == 0 || n > 4096 || probabilities.len() != n || weights.len() != n {
        return Err("input vectors must have equal length 1..4096");
    }
    let mut weighted_error = 0.0;
    let mut total = 0.0;
    for ((&y, &p), &w) in targets.iter().zip(probabilities).zip(weights) {
        if !y.is_finite() || !p.is_finite() || !w.is_finite() ||
           (y != 0.0 && y != 1.0) || !(0.0..=1.0).contains(&p) ||
           !(0.0..=1_000_000.0).contains(&w) {
            return Err("invalid binary label, predicted probability or weight");
        }
        let diff = p - y;
        weighted_error += w * diff * diff;
        total += w;
    }
    if total <= 0.0 { return Err("sample weights must sum to > 0"); }
    Ok(weighted_error / total)
}
#[cfg(test)]
mod tests {
    use super::weighted_brier_score as brier;
    #[test] fn examples() {
        assert_eq!(brier(&[0.,1.],&[0.5,0.],&[1.,3.]),Ok(0.8125));
        assert_eq!(brier(&[0.,1.],&[0.,1.],&[1.,3.]),Ok(0.0));
        assert_eq!(brier(&[0.,1.],&[0.5,0.],&[0.,1.]),Ok(1.0));
        assert_eq!(brier(&[0.],&[0.5],&[1.]),Ok(0.25));
    }
    #[test] fn invalid() {
        assert!(brier(&[],&[],&[]).is_err());
        assert!(brier(&[0.],&[0.5],&[]).is_err());
        assert!(brier(&[0.],&[0.5],&[0.]).is_err());
        assert!(brier(&[2.],&[0.5],&[1.]).is_err());
        assert!(brier(&[0.],&[1.1],&[1.]).is_err());
        assert!(brier(&[0.],&[0.5],&[f64::NAN]).is_err());
    }
}
