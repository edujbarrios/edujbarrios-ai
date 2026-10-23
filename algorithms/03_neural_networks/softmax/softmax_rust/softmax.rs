//! Stable forward softmax for a finite, nonempty vector.

/// Return a probability distribution or an explicit input error.
pub fn softmax(logits: &[f64]) -> Result<Vec<f64>, &'static str> {
    if logits.is_empty() || !logits.iter().all(|v| v.is_finite()) {
        return Err("logits must be nonempty and finite");
    }
    let maximum = logits.iter().copied().fold(f64::NEG_INFINITY, f64::max);
    let weights: Vec<f64> = logits.iter().map(|&x| (x - maximum).exp()).collect();
    let total: f64 = weights.iter().sum();
    if !total.is_finite() || total <= 0.0 {
        return Err("invalid softmax normalizer");
    }
    Ok(weights.iter().map(|x| x / total).collect())
}

#[cfg(test)]
mod tests {
    use super::softmax;
    #[test]
    fn uniform_and_large() {
        let result = softmax(&[1., 1., 1.]).unwrap();
        assert!(result.iter().all(|x| (x - 1./3.).abs()<1e-12));
        let result = softmax(&[1000., 1001.]).unwrap();
        assert!(result[1] > result[0]);
        assert!((result.iter().sum::<f64>()-1.).abs()<1e-12);
        assert_eq!(softmax(&[1e308]),Ok(vec![1.]));
    }
    #[test]
    fn invalid() {
        assert!(softmax(&[]).is_err());
        assert!(softmax(&[1., f64::NAN]).is_err());
        assert!(softmax(&[f64::INFINITY]).is_err());
    }
}
