//! Scaled Bray-Curtis dissimilarity for nonnegative feature vectors.

/// Return dissimilarity in [0,1]; two zero vectors return 0.
pub fn bray_curtis_dissimilarity(a: &[f64], b: &[f64]) -> Result<f64, &'static str> {
    if a.is_empty() || a.len() != b.len() {
        return Err("equal nonempty feature lengths required");
    }
    let mut scale = 0.0_f64;
    for (&x, &y) in a.iter().zip(b) {
        if !x.is_finite() || !y.is_finite() || x < 0.0 || y < 0.0 {
            return Err("finite nonnegative weights required");
        }
        scale = scale.max(x.max(y));
    }
    if scale == 0.0 { return Ok(0.0); }
    let (mut numerator, mut denominator) = (0.0_f64, 0.0_f64);
    for (&x, &y) in a.iter().zip(b) {
        let x_scaled = x / scale;
        let y_scaled = y / scale;
        numerator += (x_scaled - y_scaled).abs();
        denominator += x_scaled + y_scaled;
    }
    let answer = numerator / denominator;
    if !answer.is_finite() { return Err("nonfinite dissimilarity"); }
    Ok(answer.clamp(0.0, 1.0))
}

#[cfg(test)]
mod tests {
    use super::bray_curtis_dissimilarity as bc;
    #[test]
    fn reference() {
        assert!((bc(&[1.0,2.0], &[2.0,4.0]).unwrap() - 1.0/3.0).abs() < 1e-12);
        assert!((bc(&[2.0,4.0], &[1.0,2.0]).unwrap() - 1.0/3.0).abs() < 1e-12);
        assert_eq!(bc(&[0.0,0.0], &[0.0,0.0]).unwrap(), 0.0);
        assert_eq!(bc(&[0.0,1.0], &[1.0,0.0]).unwrap(), 1.0);
        assert_eq!(bc(&[1.0,2.0], &[1.0,2.0]).unwrap(), 0.0);
        assert!((bc(&[1e308,1e308], &[1e308,0.0]).unwrap()-1.0/3.0).abs()<1e-12);
    }
    #[test]
    fn invalid() {
        assert!(bc(&[], &[]).is_err());
        assert!(bc(&[1.0], &[1.0,2.0]).is_err());
        assert!(bc(&[-1.0], &[1.0]).is_err());
        assert!(bc(&[f64::NAN], &[1.0]).is_err());
        assert!(bc(&[f64::INFINITY], &[1.0]).is_err());
    }
}
