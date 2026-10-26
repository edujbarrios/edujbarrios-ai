//! Bounded, symmetric Hellinger distance for probability vectors.

/// Return distance in [0,1] for two nonempty, normalized distributions.
pub fn hellinger_distance(p: &[f64], q: &[f64]) -> Result<f64, &'static str> {
    if p.is_empty() || p.len() != q.len() {
        return Err("equal nonempty vector lengths required");
    }
    let mut sp = 0.0;
    let mut sq = 0.0;
    let mut squared = 0.0;
    for (&a, &b) in p.iter().zip(q) {
        if !a.is_finite() || !b.is_finite() ||
           !(0.0..=1.0).contains(&a) || !(0.0..=1.0).contains(&b) {
            return Err("finite probabilities in [0,1] required");
        }
        sp += a;
        sq += b;
        let difference = a.sqrt() - b.sqrt();
        squared += difference * difference;
    }
    if (sp - 1.0_f64).abs() > 1e-9 || (sq - 1.0_f64).abs() > 1e-9 {
        return Err("probabilities must sum to one");
    }
    Ok((squared / 2.0_f64).sqrt().clamp(0.0, 1.0))
}

#[cfg(test)]
mod tests {
    use super::hellinger_distance as h;
    #[test]
    fn reference() {
        assert_eq!(h(&[0.5, 0.5], &[0.5, 0.5]).unwrap(), 0.0);
        assert_eq!(h(&[1.0, 0.0], &[0.0, 1.0]).unwrap(), 1.0);
        let expected = (1.0 - 1.0 / 2.0_f64.sqrt()).sqrt();
        assert!((h(&[1.0, 0.0], &[0.5, 0.5]).unwrap() - expected).abs() < 1e-12);
        assert!((h(&[0.5, 0.5], &[1.0, 0.0]).unwrap() - expected).abs() < 1e-12);
        assert_eq!(h(&[1.0], &[1.0]).unwrap(), 0.0);
    }
    #[test]
    fn invalid() {
        assert!(h(&[], &[]).is_err());
        assert!(h(&[1.0], &[1.0, 0.0]).is_err());
        assert!(h(&[0.4, 0.4], &[1.0, 0.0]).is_err());
        assert!(h(&[1.1, -0.1], &[1.0, 0.0]).is_err());
        assert!(h(&[f64::NAN], &[1.0]).is_err());
    }
}
