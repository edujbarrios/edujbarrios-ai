//! Symmetric trimmed mean of finite observations, with configurable tails.

/// Discard floor(n*fraction) items per sorted tail, then average.
pub fn trimmed_mean(values: &[f64], trim_fraction: f64)
    -> Result<f64, &'static str> {
    if !(1..=512).contains(&values.len()) || values.iter().any(|x| !x.is_finite()) {
        return Err("expected 1..512 finite observations");
    }
    if !trim_fraction.is_finite() || !(0.0..0.5).contains(&trim_fraction) {
        return Err("trim fraction must be in [0,0.5)");
    }
    let mut sorted = values.to_vec();
    sorted.sort_by(f64::total_cmp);
    let k = ((sorted.len() as f64) * trim_fraction).floor() as usize;
    let count = sorted.len() - 2 * k;
    let mut mean = 0.0;
    for &value in &sorted[k..sorted.len()-k] {
        mean += value / count as f64;
        if !mean.is_finite() { return Err("nonfinite trimmed mean"); }
    }
    Ok(mean)
}

#[cfg(test)]
mod tests {
    use super::trimmed_mean as mean;
    #[test]
    fn examples() {
        assert_eq!(mean(&[1.,2.,3.,4.,100.],0.2),Ok(3.));
        assert_eq!(mean(&[1.,2.,3.,100.],0.25),Ok(2.5));
        assert_eq!(mean(&[1.,2.,3.,100.],0.0),Ok(26.5));
        assert_eq!(mean(&[100.,3.,2.,1.],0.25),Ok(2.5));
        assert_eq!(mean(&[42.],0.49),Ok(42.));
        assert_eq!(mean(&[1.,2.,3.],0.49),Ok(2.));
        assert!((mean(&[7.;512],0.49).unwrap()-7.0).abs() < 1e-12);
        assert_eq!(mean(&[-1e308,1e308],0.0),Ok(0.));
        assert_eq!(mean(&[0.,2.,100.],0.3),Ok(34.));
    }
    #[test]
    fn invalid() {
        assert!(mean(&[],0.0).is_err());
        assert!(mean(&[1.;513],0.0).is_err());
        assert!(mean(&[1.],-0.1).is_err());
        assert!(mean(&[1.],0.5).is_err());
        assert!(mean(&[1.],f64::INFINITY).is_err());
        assert!(mean(&[1.],f64::NAN).is_err());
        assert!(mean(&[f64::INFINITY],0.0).is_err());
        assert!(mean(&[f64::NAN],0.0).is_err());
    }
}
