//! Apply a one-time token presence penalty to finite next-token logits.
/// Returns adjusted scores or an invalid-input error.
pub fn token_presence_penalty(logits: &[f64], history: &[usize],
                              penalty: f64) -> Result<Vec<f64>, &'static str> {
    if logits.is_empty() || logits.len() > 4096 || history.len() > 4096 ||
       !penalty.is_finite() || !(0.0..=5.0).contains(&penalty) ||
       !logits.iter().all(|x| x.is_finite() && (-1000.0..=1000.0).contains(x)) {
        return Err("invalid logits, lengths or penalty");
    }
    let mut present = vec![false; logits.len()];
    for &token in history {
        if token >= logits.len() { return Err("history contains invalid token ID"); }
        present[token] = true;
    }
    Ok(logits.iter().enumerate().map(|(i,&x)| {
        if present[i] { x - penalty } else { x }
    }).collect())
}
#[cfg(test)]
mod tests {
    use super::token_presence_penalty as adjust;
    #[test] fn duplicates_and_noop() {
        assert_eq!(adjust(&[1.0,2.0,3.0], &[0,2,2], 0.5),Ok(vec![0.5,2.0,2.5]));
        assert_eq!(adjust(&[-1.0,0.0,1.0], &[0,0], 1.0),Ok(vec![-2.0,0.0,1.0]));
        assert_eq!(adjust(&[1.0,2.0], &[], 1.0),Ok(vec![1.0,2.0]));
        assert_eq!(adjust(&[1.0,2.0], &[0], 0.0),Ok(vec![1.0,2.0]));
    }
    #[test] fn invalid() {
        assert!(adjust(&[], &[], 0.5).is_err());
        assert!(adjust(&[1.0], &[1], 0.5).is_err());
        assert!(adjust(&[1.0], &[], -1.0).is_err());
        assert!(adjust(&[1.0], &[], f64::NAN).is_err());
        assert!(adjust(&[1001.0], &[], 0.5).is_err());
        assert!(adjust(&[f64::INFINITY], &[], 0.5).is_err());
    }
}
