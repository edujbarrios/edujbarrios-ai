//! Manhattan (L1) distance between finite real vectors.

pub fn manhattan_distance(left: &[f64], right: &[f64]) -> Result<f64, &'static str> {
    if left.len() != right.len() { return Err("length mismatch"); }
    let mut total = 0.0_f64;
    for (&a, &b) in left.iter().zip(right) {
        if !a.is_finite() || !b.is_finite() { return Err("non-finite coordinate"); }
        let delta = a - b;
        if !delta.is_finite() { return Err("subtraction overflow"); }
        total += delta.abs();
        if !total.is_finite() { return Err("distance overflow"); }
    }
    Ok(total)
}

#[cfg(test)]
mod tests {
    use super::manhattan_distance as l1;

    #[test]
    fn examples() {
        assert_eq!(l1(&[1.0, 2.0, -1.0], &[4.0, -2.0, 1.0]), Ok(9.0));
        assert_eq!(l1(&[1.0], &[1.0]), Ok(0.0));
        assert_eq!(l1(&[], &[]), Ok(0.0));
        assert_eq!(l1(&[-2.0, 3.0], &[4.0, -1.0]), Ok(10.0));
    }
    #[test]
    fn invalid() {
        assert!(l1(&[1.0], &[]).is_err());
        assert!(l1(&[f64::NAN], &[0.0]).is_err());
        assert!(l1(&[f64::INFINITY], &[0.0]).is_err());
        assert!(l1(&[1e308], &[-1e308]).is_err());
        assert!(l1(&[1e308, 1e308], &[0.0, 0.0]).is_err());
    }
}
