//! Scale-stable harmonic mean: positive reciprocals normalized by the minimum.

/// Harmonic mean of 1..512 finite nonnegative observations, zero absorbing.
pub fn harmonic_mean(values: &[f64]) -> Result<f64, &'static str> {
    if !(1..=512).contains(&values.len())
        || values.iter().any(|&x| !x.is_finite() || x < 0.0) {
        return Err("expected 1..512 finite nonnegative observations");
    }
    let minimum = values.iter().copied().fold(f64::INFINITY, f64::min);
    if minimum == 0.0 { return Ok(0.0); }
    let mut scaled_sum = 0.0f64;
    for &x in values { scaled_sum += minimum / x; }
    let result = minimum / (scaled_sum / values.len() as f64);
    if !result.is_finite() { return Err("nonfinite harmonic mean"); }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::harmonic_mean as mean;
    fn near(x: &[f64], expected: f64) {
        let got = mean(x).unwrap();
        assert!((got - expected).abs() <= 1e-11f64 * expected.abs().max(1e-320));
    }
    #[test]
    fn examples() {
        near(&[2.,6.],3.);
        near(&[1.,2.,4.],12./7.);
        near(&[0.,2.,6.],0.);
        near(&[42.],42.);
        near(&[7.;512],7.);
        near(&[1e-300,1e300],2e-300);
        near(&[1e-320,1e-320],1e-320);
        near(&[1e308,1e308],1e308);
    }
    #[test]
    fn invalid() {
        assert!(mean(&[]).is_err());
        assert!(mean(&[1.;513]).is_err());
        assert!(mean(&[-1.]).is_err());
        assert!(mean(&[0.,f64::NAN]).is_err());
        assert!(mean(&[1.,f64::INFINITY]).is_err());
        assert!(mean(&[f64::NEG_INFINITY]).is_err());
    }
}
