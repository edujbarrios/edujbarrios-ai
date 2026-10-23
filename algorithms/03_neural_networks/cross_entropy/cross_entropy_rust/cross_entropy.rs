//! Stable single-example multiclass cross-entropy from logits.

/// Use a zero-based target, stable max-shifted exponentials.
pub fn cross_entropy(logits: &[f64], target: usize) -> Result<f64, &'static str> {
    if logits.is_empty() || target >= logits.len() ||
        !logits.iter().all(|x| x.is_finite()) {
        return Err("invalid logits or target index");
    }
    let maximum = logits.iter().copied().fold(f64::NEG_INFINITY, f64::max);
    let sum: f64 = logits.iter().map(|&x| (x - maximum).exp()).sum();
    let delta = logits[target] - maximum;
    let loss = sum.ln() - delta;
    if !sum.is_finite() || !delta.is_finite() || !loss.is_finite() {
        return Err("cross entropy not representable");
    }
    Ok(loss.max(0.0))
}

#[cfg(test)]
mod tests {
    use super::cross_entropy;

    #[test]
    fn typical() {
        assert!((cross_entropy(&[0.,0.],0).unwrap()-2f64.ln()).abs()<1e-12);
        assert!(cross_entropy(&[1000.,0.],0).unwrap().abs()<1e-12);
        assert!((cross_entropy(&[1000.,0.],1).unwrap()-1000.).abs()<1e-12);
    }
    #[test]
    fn invalid() {
        assert!(cross_entropy(&[],0).is_err());
        assert!(cross_entropy(&[1.,2.],2).is_err());
        assert!(cross_entropy(&[f64::NAN],0).is_err());
        assert!(cross_entropy(&[f64::INFINITY],0).is_err());
    }
}
