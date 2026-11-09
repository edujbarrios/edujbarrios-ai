//! Scaled root mean square magnitude.

/// RMS of 1..512 finite observations without overflow-prone raw squares.
pub fn root_mean_square(values: &[f64]) -> Result<f64, &'static str> {
    if !(1..=512).contains(&values.len()) || values.iter().any(|x| !x.is_finite()) {
        return Err("expected 1..512 finite observations");
    }
    let scale = values.iter().fold(0.0f64, |s, x| s.max(x.abs()));
    if scale == 0.0 { return Ok(0.0); }
    let mut squares = 0.0f64;
    for &x in values {
        let y = x / scale;
        squares += y*y;
    }
    let result = scale * (squares / values.len() as f64).sqrt();
    if !result.is_finite() { return Err("nonfinite RMS"); }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::root_mean_square as rms;
    fn near(v: &[f64], want: f64) {
        let got = rms(v).unwrap();
        assert!((got-want).abs() <= 1e-11f64 * want.abs().max(1e-300));
    }
    #[test]
    fn examples() {
        near(&[3.,4.],12.5f64.sqrt());
        near(&[-3.,4.],12.5f64.sqrt());
        near(&[0.,0.,0.],0.);
        near(&[-42.],42.);
        near(&[1e308,-1e308],1e308);
        near(&[1e-300,-1e-300],1e-300);
        near(&[3.;512],3.);
        near(&[1.,2.,2.],3f64.sqrt());
    }
    #[test]
    fn invalid() {
        assert!(rms(&[]).is_err());
        assert!(rms(&[1.;513]).is_err());
        assert!(rms(&[f64::NAN]).is_err());
        assert!(rms(&[f64::INFINITY]).is_err());
        assert!(rms(&[f64::NEG_INFINITY]).is_err());
    }
}
