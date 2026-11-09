//! Geometric mean via log-domain accumulation.

/// Geometric mean of 1..512 finite nonnegative observations.
pub fn geometric_mean(values: &[f64]) -> Result<f64, &'static str> {
    if !(1..=512).contains(&values.len()) ||
        values.iter().any(|&v| !v.is_finite() || v < 0.0) {
        return Err("expected 1..512 finite nonnegative observations");
    }
    if values.contains(&0.0) { return Ok(0.0); }
    let mut log_sum = 0.0f64;
    for &v in values {
        log_sum += v.ln();
        if !log_sum.is_finite() { return Err("nonfinite log sum"); }
    }
    let result = (log_sum / values.len() as f64).exp();
    if !result.is_finite() { return Err("nonfinite geometric mean"); }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::geometric_mean as gm;
    fn near(v: &[f64], expected: f64) {
        let actual = gm(v).unwrap();
        assert!((actual - expected).abs() <= 1e-11 * expected.abs().max(1.0));
    }
    #[test]
    fn examples() {
        near(&[2.,8.],4.);
        near(&[1.,1.,1.],1.);
        near(&[0.,2.,8.],0.);
        near(&[1e-300,1e300],1.);
        near(&[2.,4.,8.],4.);
        near(&[0.5,2.],1.);
        near(&[42.],42.);
        near(&[7.;512],7.);
        near(&[0.,1e308],0.);
    }
    #[test]
    fn invalid() {
        assert!(gm(&[]).is_err());
        assert!(gm(&[1.;513]).is_err());
        assert!(gm(&[-1.]).is_err());
        assert!(gm(&[1.,f64::NAN]).is_err());
        assert!(gm(&[1.,f64::INFINITY]).is_err());
        assert!(gm(&[1.,f64::NEG_INFINITY]).is_err());
    }
}
