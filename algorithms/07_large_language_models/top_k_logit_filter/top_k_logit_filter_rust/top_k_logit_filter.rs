//! Deterministic score-and-ID top-k next-token logit filtering.

/// Keep exactly k highest-logit token IDs, breaking ties by smaller ID.
pub fn top_k_logit_filter(logits: &[f64], k: usize)
    -> Result<Vec<bool>, &'static str> {
    if !(1..=4096).contains(&logits.len()) || k < 1 || k > logits.len() ||
       logits.iter().any(|x| !x.is_finite() || x.abs() > 1000.0) {
        return Err("invalid vocabulary, logit, or k");
    }
    let mut order: Vec<usize> = (0..logits.len()).collect();
    order.sort_by(|&a,&b| logits[b].total_cmp(&logits[a]).then(a.cmp(&b)));
    let mut keep = vec![false; logits.len()];
    for &id in order.iter().take(k) {
        keep[id] = true;
    }
    Ok(keep)
}

#[cfg(test)]
mod tests {
    use super::top_k_logit_filter as filt;
    #[test]
    fn reference_cases() {
        assert_eq!(filt(&[2.0,5.0,5.0,-1.0],2),
                   Ok(vec![false,true,true,false]));
        assert_eq!(filt(&[2.0,5.0,5.0,-1.0],1),
                   Ok(vec![false,true,false,false]));
        assert_eq!(filt(&[2.0,5.0,5.0,-1.0],4),Ok(vec![true;4]));
        assert_eq!(filt(&[-2.0,-1.0,-3.0,-1.0],2),
                   Ok(vec![false,true,false,true]));
        assert_eq!(filt(&[-3.0],1),Ok(vec![true]));
    }
    #[test]
    fn invalid_cases() {
        assert!(filt(&[],1).is_err());
        assert!(filt(&vec![1.0;4097],1).is_err());
        assert!(filt(&[1.0],0).is_err());
        assert!(filt(&[1.0],2).is_err());
        assert!(filt(&[f64::NAN],1).is_err());
        assert!(filt(&[f64::INFINITY],1).is_err());
        assert!(filt(&[1001.0],1).is_err());
    }
}
