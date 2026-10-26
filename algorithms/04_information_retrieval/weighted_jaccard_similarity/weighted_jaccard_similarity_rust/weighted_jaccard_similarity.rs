//! Weighted Jaccard min/max similarity for aligned nonnegative features.

/// Return overlap in [0,1]. Identical zero vectors have similarity 1.
pub fn weighted_jaccard_similarity(a: &[f64], b: &[f64]) -> Result<f64, &'static str> {
    if a.is_empty() || a.len() != b.len() {
        return Err("equal nonempty feature vectors required");
    }
    let mut scale = 0.0_f64;
    for (&x, &y) in a.iter().zip(b) {
        if !x.is_finite() || !y.is_finite() || x < 0.0 || y < 0.0 {
            return Err("finite nonnegative weights required");
        }
        scale = scale.max(x.max(y));
    }
    if scale == 0.0 {
        return Ok(1.0);
    }
    let mut min_sum = 0.0_f64;
    let mut max_sum = 0.0_f64;
    for (&x, &y) in a.iter().zip(b) {
        min_sum += x.min(y) / scale;
        max_sum += x.max(y) / scale;
    }
    let score = min_sum / max_sum;
    if !score.is_finite() {
        return Err("nonfinite similarity");
    }
    Ok(score.clamp(0.0, 1.0))
}

#[cfg(test)]
mod tests {
    use super::weighted_jaccard_similarity as wj;

    #[test]
    fn fixtures() {
        assert!((wj(&[1.0,2.0,0.0], &[1.0,1.0,3.0]).unwrap() - 1.0/3.0).abs() < 1e-12);
        assert_eq!(wj(&[0.0,0.0], &[0.0,0.0]).unwrap(), 1.0);
        assert_eq!(wj(&[1.0,0.0], &[0.0,1.0]).unwrap(), 0.0);
        assert_eq!(wj(&[1.0,2.0], &[1.0,2.0]).unwrap(), 1.0);
        assert!((wj(&[0.2,0.8], &[0.5,0.5]).unwrap() - 7.0/13.0).abs() < 1e-12);
        assert_eq!(wj(&[1e308,1e308], &[1e308,0.0]).unwrap(), 0.5);
    }

    #[test]
    fn invalid() {
        assert!(wj(&[], &[]).is_err());
        assert!(wj(&[1.0], &[1.0,2.0]).is_err());
        assert!(wj(&[-1.0], &[0.0]).is_err());
        assert!(wj(&[f64::NAN], &[0.0]).is_err());
        assert!(wj(&[f64::INFINITY], &[0.0]).is_err());
    }
}
