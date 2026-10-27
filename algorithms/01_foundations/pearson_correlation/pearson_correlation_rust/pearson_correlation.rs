//! Pearson correlation of bounded paired finite real observations.

/// Return a product-moment correlation; undefined for constant features.
pub fn pearson_correlation(x: &[f64], y: &[f64]) -> Result<f64, &'static str> {
    let n = x.len();
    if !(2..=10_000).contains(&n) || n != y.len() {
        return Err("equal lengths from 2 to 10000 required");
    }
    if x.iter().chain(y.iter()).any(|v| !v.is_finite() || v.abs() > 1_000_000.0) {
        return Err("values must be finite and bounded");
    }
    let mx = x.iter().sum::<f64>() / n as f64;
    let my = y.iter().sum::<f64>() / n as f64;
    let (mut sxx, mut syy, mut sxy) = (0.0, 0.0, 0.0);
    for (&a, &b) in x.iter().zip(y.iter()) {
        let dx = a - mx;
        let dy = b - my;
        sxx += dx * dx;
        syy += dy * dy;
        sxy += dx * dy;
    }
    if sxx <= 0.0 || syy <= 0.0 {
        return Err("correlation undefined for constant features");
    }
    let r = sxy / sxx.sqrt() / syy.sqrt();
    if !r.is_finite() {
        return Err("nonfinite result");
    }
    Ok(r.clamp(-1.0, 1.0))
}

#[cfg(test)]
mod tests {
    use super::pearson_correlation as correlation;

    fn close(actual: f64, expected: f64) {
        assert!((actual - expected).abs() < 1e-12, "{actual} != {expected}");
    }

    #[test]
    fn reference_cases() {
        close(correlation(&[1.0, 2.0, 3.0], &[2.0, 4.0, 6.0]).unwrap(), 1.0);
        close(correlation(&[1.0, 2.0, 3.0], &[6.0, 4.0, 2.0]).unwrap(), -1.0);
        close(correlation(&[1.0, 2.0, 3.0], &[1.0, 0.0, 1.0]).unwrap(), 0.0);
        close(correlation(&[1.0, 2.0], &[3.0, 4.0]).unwrap(), 1.0);
    }

    #[test]
    fn rejects_bad_inputs() {
        assert!(correlation(&[], &[]).is_err());
        assert!(correlation(&[1.0], &[2.0]).is_err());
        assert!(correlation(&[1.0, 2.0], &[1.0]).is_err());
        assert!(correlation(&[1.0, 2.0], &[4.0, 4.0]).is_err());
        assert!(correlation(&[1.0, 2.0], &[1.0, f64::NAN]).is_err());
        assert!(correlation(&[1.0, 2.0], &[1.0, f64::INFINITY]).is_err());
        assert!(correlation(&[1.0, 2.0], &[1.0, 1_000_001.0]).is_err());
    }
}
