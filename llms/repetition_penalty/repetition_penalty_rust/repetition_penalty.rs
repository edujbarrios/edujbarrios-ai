//! Sign-aware multiplicative penalty on logits of generated token IDs.

/// Adjust seen-token logits, with each distinct ID penalized exactly once.
pub fn repetition_penalty(logits: &[f64], history: &[usize], penalty: f64)
    -> Result<Vec<f64>, &'static str> {
    if !(1..=4096).contains(&logits.len()) || history.len() > 16_384 ||
       !penalty.is_finite() || !(1.0..=10.0).contains(&penalty) ||
       logits.iter().any(|v| !v.is_finite() || v.abs() > 1000.0) ||
       history.iter().any(|&token| token >= logits.len()) {
        return Err("invalid logits, history, or penalty");
    }
    let mut result = logits.to_vec();
    let mut seen = vec![false; logits.len()];
    for &token in history {
        seen[token] = true;
    }
    for (i, value) in result.iter_mut().enumerate() {
        if seen[i] {
            *value = if *value > 0.0 { *value / penalty } else { *value * penalty };
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::repetition_penalty as adjust;
    #[test]
    fn reference() {
        let x = [2.,-2.,0.,1.];
        assert_eq!(adjust(&x,&[0,1,1,2],2.),Ok(vec![1.,-4.,0.,1.]));
        assert_eq!(adjust(&x,&[0,1,1,2],1.),Ok(x.to_vec()));
        assert_eq!(adjust(&x,&[],10.),Ok(x.to_vec()));
        assert_eq!(adjust(&[1.],&[0],10.),Ok(vec![0.1]));
    }
    #[test]
    fn invalid() {
        let x = [2.,-2.,0.,1.];
        assert!(adjust(&x,&[4],2.).is_err());
        assert!(adjust(&x,&[0],0.5).is_err());
        assert!(adjust(&x,&[0],11.).is_err());
        assert!(adjust(&x,&[],f64::NAN).is_err());
        assert!(adjust(&[],&[],2.).is_err());
        assert!(adjust(&[1001.],&[],2.).is_err());
        assert!(adjust(&[f64::INFINITY],&[],2.).is_err());
        assert!(adjust(&x,&[0;16385],2.).is_err());
    }
}
