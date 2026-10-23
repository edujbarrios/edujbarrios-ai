//! Graded relevance NDCG@K using exponential gain.

fn dcg(grades: &[u8], k: usize) -> f64 {
    grades.iter().take(k).enumerate().map(|(i, &r)| {
        ((1_u64 << r) - 1) as f64 / ((i + 2) as f64).log2()
    }).sum()
}

/// Calculate NDCG at a cutoff using integer grades 0..20 inclusive.
pub fn ndcg_at_k(grades: &[u8], k: usize) -> Result<f64, &'static str> {
    if grades.is_empty() || k == 0 || k > grades.len() {
        return Err("ranking must be nonempty and contain at least k results");
    }
    if grades.iter().any(|&r| r > 20) {
        return Err("grades must be within 0..20");
    }
    let current = dcg(grades, k);
    let mut ideal = grades.to_vec();
    ideal.sort_unstable_by(|a, b| b.cmp(a));
    let maximum = dcg(&ideal, k);
    if !current.is_finite() || !maximum.is_finite() {
        return Err("non-finite gain");
    }
    if maximum == 0.0 {
        return Ok(0.0);
    }
    Ok((current / maximum).clamp(0.0, 1.0))
}

#[cfg(test)]
mod tests {
    use super::ndcg_at_k;

    #[test]
    fn ideal_and_nonideal() {
        assert_eq!(ndcg_at_k(&[3, 2, 1, 0], 3), Ok(1.0));
        let value = ndcg_at_k(&[3, 2, 0, 1], 3).unwrap();
        assert!(value > 0.9 && value < 1.0);
        assert_eq!(ndcg_at_k(&[0, 0, 0], 2), Ok(0.0));
    }

    #[test]
    fn invalid() {
        assert!(ndcg_at_k(&[], 1).is_err());
        assert!(ndcg_at_k(&[1], 0).is_err());
        assert!(ndcg_at_k(&[1], 2).is_err());
        assert!(ndcg_at_k(&[1, 2, 21], 1).is_err());
    }
}
