//! Apply an occurrence-count penalty to next-token logits.

/// Return logits minus penalty times each token's history frequency.
pub fn token_frequency_penalty(logits: &[f64], history: &[usize], penalty: f64)
    -> Result<Vec<f64>, &'static str> {
    if !(1..=4096).contains(&logits.len()) || history.len()>4096 ||
       !penalty.is_finite() || !(0.0..=5.0).contains(&penalty) ||
       logits.iter().any(|v|!v.is_finite() || v.abs()>1000.0) ||
       history.iter().any(|&i|i>=logits.len()) {
        return Err("invalid vocabulary, history, or penalty");
    }
    let mut counts=vec![0_usize;logits.len()];
    for &id in history {counts[id]+=1;}
    Ok(logits.iter().zip(counts.iter())
        .map(|(value,count)|value-penalty*(*count as f64)).collect())
}

#[cfg(test)]
mod tests {
    use super::token_frequency_penalty as penalize;
    #[test]
    fn reference() {
        assert_eq!(penalize(&[1.0,2.0,3.0],&[0,2,2],0.5),
                   Ok(vec![0.5,2.0,2.0]));
        assert_eq!(penalize(&[-1.0,0.0,1.0],&[0,0],1.0),
                   Ok(vec![-3.0,0.0,1.0]));
        assert_eq!(penalize(&[1.0,2.0,3.0],&[],1.0),
                   Ok(vec![1.0,2.0,3.0]));
        assert_eq!(penalize(&[1.0,2.0,3.0],&[0,2,2],0.0),
                   Ok(vec![1.0,2.0,3.0]));
    }
    #[test]
    fn invalid() {
        assert!(penalize(&[],&[],1.0).is_err());
        assert!(penalize(&[1.0,2.0,3.0],&[3],1.0).is_err());
        assert!(penalize(&[1.0,2.0,3.0],&vec![0;4097],1.0).is_err());
        assert!(penalize(&[1.0],&[], -1.0).is_err());
        assert!(penalize(&[1.0],&[], 6.0).is_err());
        assert!(penalize(&[1.0],&[], f64::NAN).is_err());
        assert!(penalize(&[f64::NAN],&[],1.0).is_err());
        assert!(penalize(&[1001.0],&[],1.0).is_err());
    }
}
