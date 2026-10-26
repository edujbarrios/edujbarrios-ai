//! Asymmetric Tversky index for sets of signed 32-bit IDs.

/// Return similarity in [0,1] for finite, strictly positive alpha and beta.
pub fn tversky_index(first: &[i32], second: &[i32],
                    alpha: f64, beta: f64) -> Result<f64, &'static str> {
    if !alpha.is_finite() || !beta.is_finite() || alpha <= 0.0 || beta <= 0.0 {
        return Err("strictly positive finite parameters required");
    }
    let mut a = first.to_vec();
    let mut b = second.to_vec();
    a.sort_unstable();
    b.sort_unstable();
    a.dedup();
    b.dedup();
    if a.is_empty() && b.is_empty() {return Ok(1.0);}
    if a.is_empty() || b.is_empty() {return Ok(0.0);}
    let (mut i, mut j, mut common) = (0_usize, 0_usize, 0_usize);
    while i < a.len() && j < b.len() {
        if a[i] == b[j] {i += 1; j += 1; common += 1;}
        else if a[i] < b[j] {i += 1;}
        else {j += 1;}
    }
    let denom = common as f64 + alpha * (a.len() - common) as f64 +
        beta * (b.len() - common) as f64;
    if !denom.is_finite() || denom <= 0.0 {
        return Err("unrepresentable denominator");
    }
    Ok((common as f64 / denom).clamp(0.0, 1.0))
}

#[cfg(test)]
mod tests {
    use super::tversky_index as tv;
    #[test]
    fn cases() {
        assert_eq!(tv(&[1,2], &[2,3,4], 1.0, 1.0).unwrap(), 0.25);
        assert!((tv(&[1,2], &[2,3,4], 0.5, 2.0).unwrap()-2.0/11.0).abs()<1e-12);
        assert_eq!(tv(&[2,3,4], &[1,2], 0.5, 2.0).unwrap(), 0.25);
        assert_eq!(tv(&[1,1,2], &[1,2], 1.0, 1.0).unwrap(), 1.0);
        assert_eq!(tv(&[], &[], 1.0, 1.0).unwrap(), 1.0);
        assert_eq!(tv(&[], &[1], 1.0, 1.0).unwrap(), 0.0);
        assert_eq!(tv(&[i32::MIN], &[i32::MAX, i32::MIN], 1.0, 1.0).unwrap(), 0.5);
    }
    #[test]
    fn invalid() {
        assert!(tv(&[], &[], 0.0, 1.0).is_err());
        assert!(tv(&[], &[], 1.0, f64::NAN).is_err());
        assert!(tv(&[1,2], &[2,3], 1e308, 1e308).is_err());
    }
}
