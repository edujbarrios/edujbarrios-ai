//! Filter LLM next-token logits by a threshold relative to the maximum.

/// Compute a bool keep mask by stable exp(score - max_score).
pub fn min_p_logit_filter(logits: &[f64], min_p: f64)
    -> Result<Vec<bool>, &'static str> {
    if !(1..=4096).contains(&logits.len()) ||
       !min_p.is_finite() || !(0.0..=1.0).contains(&min_p) ||
       logits.iter().any(|x| !x.is_finite() || x.abs() > 1000.0) {
        return Err("invalid logits or min_p");
    }
    let maximum = logits.iter().copied().fold(f64::NEG_INFINITY, f64::max);
    Ok(logits.iter().map(|x|
        min_p == 0.0 || (x - maximum).exp() >= min_p).collect())
}

#[cfg(test)]
mod tests {
    use super::min_p_logit_filter as filt;
    #[test]
    fn reference() {
        assert_eq!(filt(&[0.0,-1.0,-2.0],0.3),Ok(vec![true,true,false]));
        assert_eq!(filt(&[5.0,5.0,0.0],1.0),Ok(vec![true,true,false]));
        assert_eq!(filt(&[-1000.0,1000.0],0.0),Ok(vec![true,true]));
        assert_eq!(filt(&[-1000.0,1000.0],0.5),Ok(vec![false,true]));
        assert_eq!(filt(&[-5.0],1.0),Ok(vec![true]));
    }
    #[test]
    fn invalid() {
        assert!(filt(&[],0.5).is_err());
        assert!(filt(&vec![0.0;4097],0.5).is_err());
        assert!(filt(&[0.0],-0.1).is_err());
        assert!(filt(&[0.0],1.1).is_err());
        assert!(filt(&[0.0],f64::NAN).is_err());
        assert!(filt(&[f64::NAN],0.5).is_err());
        assert!(filt(&[1001.0],0.5).is_err());
    }
}
