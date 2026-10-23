//! Recall@k over a ranked sequence of binary relevance labels.

pub fn recall_at_k(
    relevance: &[u8], k: usize, total_relevant: usize
) -> Result<f64, &'static str> {
    if relevance.is_empty() || k == 0 || k > relevance.len() ||
       total_relevant == 0 {
        return Err("invalid ranking length, k or total relevant");
    }
    let mut observed = 0_usize;
    let mut top_relevant = 0_usize;
    for (i, &label) in relevance.iter().enumerate() {
        if label > 1 {
            return Err("relevance labels must be binary");
        }
        if label == 1 {
            observed += 1;
            if i < k { top_relevant += 1; }
        }
    }
    if observed > total_relevant {
        return Err("observed relevant exceeds total relevant");
    }
    Ok(top_relevant as f64 / total_relevant as f64)
}

#[cfg(test)]
mod tests {
    use super::recall_at_k;
    #[test]
    fn values() {
        assert!((recall_at_k(&[1,0,1,0],3,3).unwrap() - 2.0/3.0).abs() < 1e-12);
        assert!((recall_at_k(&[1,0,1,0],1,3).unwrap() - 1.0/3.0).abs() < 1e-12);
        assert_eq!(recall_at_k(&[0,0],2,5),Ok(0.0));
    }
    #[test]
    fn invalid() {
        assert!(recall_at_k(&[],1,3).is_err());
        assert!(recall_at_k(&[1,0],0,3).is_err());
        assert!(recall_at_k(&[1,0],3,3).is_err());
        assert!(recall_at_k(&[1,0],1,0).is_err());
        assert!(recall_at_k(&[1,0,1],2,1).is_err());
        assert!(recall_at_k(&[1,0,2],1,3).is_err());
    }
}
