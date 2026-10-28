//! Gini impurity over bounded nonnegative class counts.

/// Compute 1 - sum((count/total)^2) with total > 0.
pub fn gini_impurity(counts: &[u32]) -> Result<f64, &'static str> {
    if !(1..=128).contains(&counts.len()) ||
       counts.iter().any(|&v| v > 1_000_000) {
        return Err("1..128 counts, each at most 1000000, required");
    }
    let total: u64 = counts.iter().map(|&v| u64::from(v)).sum();
    if total == 0 { return Err("at least one positive count is required"); }
    let result = 1.0 - counts.iter()
        .map(|&v| (f64::from(v)/total as f64).powi(2))
        .sum::<f64>();
    Ok(result.clamp(0.0,1.0))
}

#[cfg(test)]
mod tests {
    use super::gini_impurity as gini;
    #[test]
    fn examples() {
        assert_eq!(gini(&[5,0]),Ok(0.0));
        assert_eq!(gini(&[1,1]),Ok(0.5));
        assert_eq!(gini(&[3,1]),Ok(0.375));
        assert!((gini(&[1,1,1]).unwrap()-2.0/3.0).abs()<1e-12);
        assert_eq!(gini(&[1000000,1000000]),Ok(0.5));
        assert_eq!(gini(&[8]),Ok(0.0));
    }
    #[test]
    fn invalid() {
        assert!(gini(&[]).is_err());
        assert!(gini(&[0,0]).is_err());
        assert!(gini(&[0,1000001]).is_err());
        assert!(gini(&vec![1;129]).is_err());
    }
}
