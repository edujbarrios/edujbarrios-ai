//! Mean reciprocal rank of first relevant retrieval positions.

pub fn mean_reciprocal_rank(ranks: &[usize]) -> Result<f64, &'static str> {
    if ranks.is_empty() {
        return Err("at least one query rank is required");
    }
    let mut total: f64 = 0.0;
    for &rank in ranks {
        if rank > 0 {
            total += 1.0 / rank as f64;
            if !total.is_finite() {
                return Err("non-finite sum");
            }
        }
    }
    let score = total / ranks.len() as f64;
    if !score.is_finite() || !(0.0..=1.0).contains(&score) {
        return Err("non-finite reciprocal rank");
    }
    Ok(score)
}

#[cfg(test)]
mod tests {
    use super::mean_reciprocal_rank as mrr;
    #[test]
    fn values() {
        assert!((mrr(&[1,2,0,4]).unwrap()-0.4375).abs()<1e-12);
        assert_eq!(mrr(&[0,0,0]),Ok(0.0));
        assert_eq!(mrr(&[1]),Ok(1.0));
    }
    #[test]
    fn invalid() {
        assert!(mrr(&[]).is_err());
    }
}
