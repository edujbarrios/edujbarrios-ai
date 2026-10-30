//! Sparse additive logit bias for selected vocabulary tokens.
/// Return adjusted logits, rejecting nonfinite data, invalid IDs and duplicates.
pub fn logit_bias_adjustment(logits: &[f64], token_ids: &[usize],
                             biases: &[f64]) -> Result<Vec<f64>, &'static str> {
    let n = logits.len();
    if n == 0 || n > 4096 || token_ids.len() != biases.len() ||
       token_ids.len() > n ||
       !logits.iter().all(|x| x.is_finite() && x.abs() <= 1000.0) {
        return Err("invalid vocabulary size, logits or sparse bias lengths");
    }
    let mut seen = vec![false; n];
    for (&token, &bias) in token_ids.iter().zip(biases) {
        if token >= n || seen[token] || !bias.is_finite() || bias.abs() > 100.0 {
            return Err("invalid or duplicate bias token");
        }
        seen[token] = true;
    }
    let mut result = logits.to_vec();
    for (&token, &bias) in token_ids.iter().zip(biases) {
        result[token] += bias;
    }
    Ok(result)
}
#[cfg(test)]
mod tests {
    use super::logit_bias_adjustment as adjust;
    #[test] fn examples() {
        assert_eq!(adjust(&[1.,2.,3.],&[0,2],&[-1.,0.5]),Ok(vec![0.,2.,3.5]));
        assert_eq!(adjust(&[-1.,0.],&[],&[]),Ok(vec![-1.,0.]));
        assert_eq!(adjust(&[1.,2.],&[1],&[0.]),Ok(vec![1.,2.]));
        assert_eq!(adjust(&[-1000.,1000.],&[0,1],&[-100.,100.]),Ok(vec![-1100.,1100.]));
    }
    #[test] fn invalid() {
        assert!(adjust(&[],&[],&[]).is_err());
        assert!(adjust(&[1.],&[0,0],&[1.,1.]).is_err());
        assert!(adjust(&[1.],&[1],&[1.]).is_err());
        assert!(adjust(&[1.],&[0],&[f64::NAN]).is_err());
        assert!(adjust(&[1001.],&[],&[]).is_err());
        assert!(adjust(&[1.],&[0],&[]).is_err());
    }
}
