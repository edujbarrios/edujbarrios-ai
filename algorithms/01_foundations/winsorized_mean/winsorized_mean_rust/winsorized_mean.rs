//! Symmetric winsorized mean, replacing both sorted tails before averaging.

/// Clamp floor(n*f) observations from each tail and average all observations.
pub fn winsorized_mean(values: &[f64], trim_fraction: f64)
    -> Result<f64, &'static str> {
    if !(1..=512).contains(&values.len()) || values.iter().any(|v| !v.is_finite()) {
        return Err("expected 1..512 finite observations");
    }
    if !trim_fraction.is_finite() || !(0.0..0.5).contains(&trim_fraction) {
        return Err("trim fraction must be in [0,0.5)");
    }
    let mut sorted = values.to_vec();
    sorted.sort_by(f64::total_cmp);
    let n = sorted.len();
    let k = ((n as f64) * trim_fraction).floor() as usize;
    let (low, high) = (sorted[k], sorted[n-k-1]);
    let mut result = 0.0f64;
    for (i, &v) in sorted.iter().enumerate() {
        let clipped = if i < k {low} else if i >= n-k {high} else {v};
        result += clipped / n as f64;
        if !result.is_finite() {return Err("nonfinite winsorized mean");}
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::winsorized_mean as wmean;
    fn near(v:&[f64],f:f64,expected:f64) {
        let actual=wmean(v,f).unwrap();
        assert!((actual-expected).abs()<=1e-12f64*expected.abs().max(1.0));
    }
    #[test]
    fn examples() {
        near(&[1.,2.,3.,4.,100.],0.2,3.);
        near(&[1.,2.,3.,100.],0.25,2.5);
        near(&[1.,2.,3.,100.],0.,26.5);
        near(&[100.,3.,2.,1.],0.25,2.5);
        near(&[1.,5.,100.],0.49,5.);
        near(&[42.],0.49,42.);
        near(&[7.;512],0.49,7.);
        near(&[-1e308,1e308],0.,0.);
        near(&[0.,2.,100.],0.3,34.);
        near(&[1.,2.,3.,100.,1000.],0.2,41.6);
    }
    #[test]
    fn invalid() {
        assert!(wmean(&[],0.).is_err());
        assert!(wmean(&[1.;513],0.).is_err());
        assert!(wmean(&[1.],-0.1).is_err());
        assert!(wmean(&[1.],0.5).is_err());
        assert!(wmean(&[1.],f64::INFINITY).is_err());
        assert!(wmean(&[1.],f64::NAN).is_err());
        assert!(wmean(&[f64::INFINITY],0.).is_err());
        assert!(wmean(&[f64::NAN],0.).is_err());
    }
}
