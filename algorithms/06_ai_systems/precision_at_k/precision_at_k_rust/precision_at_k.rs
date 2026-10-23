//! Precision@K for a ranked sequence of binary relevance judgments.

pub fn precision_at_k(relevance: &[u8], k: usize) -> Result<f64, &'static str> {
    if relevance.is_empty() || k == 0 || k > relevance.len() {
        return Err("k must be between 1 and number of results");
    }
    let mut relevant: usize = 0;
    for (index, &label) in relevance.iter().enumerate() {
        if label > 1 {
            return Err("relevance judgments must be 0 or 1");
        }
        if index < k {
            relevant += usize::from(label);
        }
    }
    Ok(relevant as f64 / k as f64)
}

#[cfg(test)]
mod tests {
    use super::precision_at_k;

    #[test]
    fn known_values() {
        assert!((precision_at_k(&[1, 0, 1, 1], 3).unwrap() - 2.0 / 3.0).abs() < 1e-12);
        assert_eq!(precision_at_k(&[1, 0, 1, 1], 4), Ok(0.75));
        assert_eq!(precision_at_k(&[0, 0], 2), Ok(0.0));
    }

    #[test]
    fn invalid() {
        assert!(precision_at_k(&[], 1).is_err());
        assert!(precision_at_k(&[1], 0).is_err());
        assert!(precision_at_k(&[1], 2).is_err());
        assert!(precision_at_k(&[1, 0, 2], 2).is_err());
    }
}
