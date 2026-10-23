//! Reciprocal rank fusion of two rankings of nonnegative i32 IDs.

use std::collections::{HashMap, HashSet};

/// Return fused IDs ordered by descending score, then ascending ID.
pub fn reciprocal_rank_fusion(a: &[i32], b: &[i32]) -> Result<Vec<i32>, &'static str> {
    if a.is_empty() && b.is_empty() {
        return Err("at least one ranking must be nonempty");
    }
    let mut scores: HashMap<i32, f64> = HashMap::new();
    for ranking in [a, b] {
        let mut seen = HashSet::new();
        for (rank, &item) in ranking.iter().enumerate() {
            if item < 0 {
                return Err("IDs must be nonnegative");
            }
            if !seen.insert(item) {
                return Err("duplicate ID within one ranking");
            }
            *scores.entry(item).or_insert(0.0) += 1.0 / (60.0 + (rank + 1) as f64);
        }
    }
    let mut result: Vec<(i32, f64)> = scores.into_iter().collect();
    result.sort_by(|a, b| b.1.total_cmp(&a.1).then(a.0.cmp(&b.0)));
    Ok(result.into_iter().map(|(id, _)| id).collect())
}

#[cfg(test)]
mod tests {
    use super::reciprocal_rank_fusion as rrf;

    #[test]
    fn overlap_ties_one_empty() {
        assert_eq!(rrf(&[10,20,30], &[30,10,40]), Ok(vec![10,30,20,40]));
        assert_eq!(rrf(&[2,1], &[1,2]), Ok(vec![1,2]));
        assert_eq!(rrf(&[10,20,30], &[]), Ok(vec![10,20,30]));
    }

    #[test]
    fn invalid() {
        assert!(rrf(&[], &[]).is_err());
        assert!(rrf(&[1,1], &[]).is_err());
        assert!(rrf(&[-1], &[]).is_err());
    }
}
