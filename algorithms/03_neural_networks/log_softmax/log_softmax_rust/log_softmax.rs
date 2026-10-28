//! Stable log-softmax for a flat vector of finite logits.

/// Return log probabilities using max shifting; every output is finite.
pub fn log_softmax(logits: &[f64]) -> Result<Vec<f64>, &'static str> {
    if !(1..=10000).contains(&logits.len()) ||
       logits.iter().any(|x| !x.is_finite() || x.abs() > 1_000_000.0) {
        return Err("finite bounded logit vector length 1..10000 required");
    }
    let largest = logits.iter().copied().fold(f64::NEG_INFINITY, f64::max);
    let sum: f64 = logits.iter().map(|z| (z-largest).exp()).sum();
    if !sum.is_finite() || sum <= 0.0 {
        return Err("invalid exponential sum");
    }
    let correction = sum.ln();
    let result: Vec<f64> = logits.iter()
        .map(|z| (z-largest)-correction)
        .collect();
    if result.iter().any(|x| !x.is_finite()) { return Err("nonfinite result"); }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::log_softmax;
    fn close(a:f64,b:f64) {
        assert!((a-b).abs()<1e-12,"{a} != {b}");
    }
    #[test]
    fn reference() {
        let x=log_softmax(&[0.,0.]).unwrap();
        close(x[0],-2.0_f64.ln());
        close(x[1],x[0]);
        let y=log_softmax(&[0.,2.0_f64.ln()]).unwrap();
        close(y[0],-3.0_f64.ln());
        close(y[1],2.0_f64.ln()-3.0_f64.ln());
        close(y.iter().map(|v| v.exp()).sum(),1.);
        let shifted=log_softmax(&[100.,100.+2.0_f64.ln()]).unwrap();
        close(shifted[0],y[0]);close(shifted[1],y[1]);
        assert_eq!(log_softmax(&[1e6,-1e6]),Ok(vec![0.,-2e6]));
        assert_eq!(log_softmax(&[5.]),Ok(vec![0.]));
    }
    #[test]
    fn invalid() {
        assert!(log_softmax(&[]).is_err());
        assert!(log_softmax(&[f64::NAN]).is_err());
        assert!(log_softmax(&[f64::INFINITY]).is_err());
        assert!(log_softmax(&[1_000_001.]).is_err());
        assert!(log_softmax(&vec![0.;10001]).is_err());
    }
}
