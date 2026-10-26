//! Total variation distance for discrete probability distributions.

/// Return a distance in [0, 1] for two finite, normalized distributions.
pub fn total_variation_distance(p: &[f64], q: &[f64]) -> Result<f64, &'static str> {
    if p.is_empty() || p.len() != q.len() {
        return Err("equal nonempty distribution lengths required");
    }
    let (mut ps, mut qs, mut deltas) = (0.0_f64, 0.0_f64, 0.0_f64);
    for (&a, &b) in p.iter().zip(q) {
        if !a.is_finite() || !b.is_finite() ||
            !(0.0..=1.0).contains(&a) || !(0.0..=1.0).contains(&b) {
            return Err("finite probabilities in [0,1] required");
        }
        ps += a;
        qs += b;
        deltas += (a - b).abs();
    }
    if (ps - 1.0).abs() > 1e-9 || (qs - 1.0).abs() > 1e-9 {
        return Err("probabilities must sum to one");
    }
    Ok((0.5 * deltas).clamp(0.0, 1.0))
}

#[cfg(test)]
mod tests {
    use super::total_variation_distance as tv;
    #[test]
    fn fixtures() {
        assert_eq!(tv(&[0.5, 0.5], &[0.5, 0.5]).unwrap(), 0.0);
        assert_eq!(tv(&[1.0, 0.0], &[0.0, 1.0]).unwrap(), 1.0);
        assert!((tv(&[1.0, 0.0], &[0.25, 0.75]).unwrap() - 0.75).abs() < 1e-12);
        assert!((tv(&[0.25, 0.75], &[1.0, 0.0]).unwrap() - 0.75).abs() < 1e-12);
        assert!((tv(&[0.1, 0.2, 0.7], &[0.2, 0.3, 0.5]).unwrap() - 0.2).abs() < 1e-12);
        assert_eq!(tv(&[1.0], &[1.0]).unwrap(), 0.0);
    }
    #[test]
    fn invalid() {
        assert!(tv(&[], &[]).is_err());
        assert!(tv(&[1.0], &[1.0, 0.0]).is_err());
        assert!(tv(&[0.4, 0.4], &[0.5, 0.5]).is_err());
        assert!(tv(&[1.1, -0.1], &[1.0, 0.0]).is_err());
        assert!(tv(&[f64::NAN], &[1.0]).is_err());
    }
}
