//! Scale-normalized Minkowski distance for finite p >= 1.

pub fn minkowski_distance(a: &[f64], b: &[f64], p: f64) -> Result<f64, &'static str> {
    if a.is_empty() || a.len() != b.len() || !p.is_finite() || p < 1.0 {
        return Err("equal nonempty lengths and finite p >= 1 required");
    }
    let mut scale = 0.0_f64;
    for (&x, &y) in a.iter().zip(b) {
        if !x.is_finite() || !y.is_finite() {
            return Err("finite coordinates required");
        }
        let d = (x - y).abs();
        if !d.is_finite() { return Err("unrepresentable difference"); }
        scale = scale.max(d);
    }
    if scale == 0.0 { return Ok(0.0); }
    let sum: f64 = a.iter().zip(b)
        .map(|(&x, &y)| ((x - y).abs() / scale).powf(p)).sum();
    let answer = scale * sum.powf(1.0 / p);
    if !answer.is_finite() { return Err("unrepresentable distance"); }
    Ok(answer)
}

#[cfg(test)]
mod tests {
    use super::minkowski_distance as md;
    #[test]
    fn fixtures() {
        assert_eq!(md(&[0.0,0.0], &[3.0,4.0], 1.0).unwrap(), 7.0);
        assert_eq!(md(&[0.0,0.0], &[3.0,4.0], 2.0).unwrap(), 5.0);
        assert!((md(&[0.0,0.0], &[3.0,4.0], 3.0).unwrap() - 91_f64.powf(1.0/3.0)).abs() < 1e-12);
        assert_eq!(md(&[7.0], &[7.0], 2.0).unwrap(), 0.0);
        assert!((md(&[1e150,1e150], &[0.0,0.0], 2.0).unwrap()/1e150 - 2.0_f64.sqrt()).abs() < 1e-12);
        assert!((md(&[0.0,0.0], &[3.0,4.0], 1000.0).unwrap() - 4.0).abs() < 0.01);
    }
    #[test]
    fn invalid() {
        assert!(md(&[], &[], 2.0).is_err());
        assert!(md(&[1.0], &[1.0,2.0], 2.0).is_err());
        assert!(md(&[f64::NAN], &[0.0], 2.0).is_err());
        assert!(md(&[1.0], &[0.0], 0.0).is_err());
        assert!(md(&[1.0], &[0.0], f64::INFINITY).is_err());
        assert!(md(&[1e308], &[-1e308], 2.0).is_err());
    }
}
