//! Full-sort nearest signatures by Hamming distance, with stable ties.

const MAX_SIGNATURE: u32 = (1_u32 << 30) - 1;

/// Return original indices of k closest 30-bit signatures.
pub fn hamming_top_k(query: u32, candidates: &[u32], k: usize)
    -> Result<Vec<usize>, &'static str> {
    if query > MAX_SIGNATURE || candidates.is_empty() || k == 0 ||
       k > candidates.len() ||
       candidates.iter().any(|&value| value > MAX_SIGNATURE) {
        return Err("invalid signatures or k");
    }
    let mut indices: Vec<usize> = (0..candidates.len()).collect();
    indices.sort_by(|&a, &b| {
        let da = (query ^ candidates[a]).count_ones();
        let db = (query ^ candidates[b]).count_ones();
        da.cmp(&db).then(a.cmp(&b))
    });
    indices.truncate(k);
    Ok(indices)
}

#[cfg(test)]
mod tests {
    use super::hamming_top_k as search;
    #[test]
    fn rank_and_ties() {
        assert_eq!(search(3, &[1, 7, 15, 2, 3], 4), Ok(vec![4, 0, 1, 3]));
        assert_eq!(search(3, &[3, 3, 0], 2), Ok(vec![0, 1]));
        assert_eq!(search(0, &[0, (1 << 30) - 1], 2), Ok(vec![0, 1]));
    }
    #[test]
    fn invalid() {
        assert!(search(3, &[], 1).is_err());
        assert!(search(3, &[1], 0).is_err());
        assert!(search(3, &[1], 2).is_err());
        assert!(search(3, &[1, 1 << 30], 1).is_err());
        assert!(search(1 << 30, &[0], 1).is_err());
    }
}
