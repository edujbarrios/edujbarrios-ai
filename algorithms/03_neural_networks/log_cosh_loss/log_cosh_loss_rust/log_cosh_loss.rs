//! Stable scalar mean log-cosh regression loss.

/// Mean log-cosh loss with finite bounded targets and predictions.
pub fn log_cosh_loss(target: &[f64], prediction: &[f64])
    -> Result<f64, &'static str>
{
    let n = target.len();
    if !(1..=10_000).contains(&n) || prediction.len() != n {
        return Err("equal lengths in 1..10000 required");
    }
    let mut total = 0.0;
    for (&t, &p) in target.iter().zip(prediction.iter()) {
        if !t.is_finite() || !p.is_finite() ||
            t.abs() > 1_000_000.0 || p.abs() > 1_000_000.0 {
            return Err("finite inputs inside bounds required");
        }
        let a = (p - t).abs();
        let term = a + (-2.0*a).exp().ln_1p() - 2.0_f64.ln();
        total += term.max(0.0);
    }
    let loss = total / n as f64;
    if !loss.is_finite() {
        return Err("nonfinite loss");
    }
    Ok(loss)
}

#[cfg(test)]
mod tests {
    use super::log_cosh_loss as loss;
    fn close(actual: f64, expected: f64, tol: f64) {
        assert!((actual-expected).abs() < tol, "{actual} != {expected}");
    }
    #[test]
    fn reference() {
        close(loss(&[0.,1.,2.],&[0.,2.,0.]).unwrap(),0.5862611926136305,1e-12);
        close(loss(&[0.,1.,2.],&[0.,1.,2.]).unwrap(),0.0,1e-12);
        close(loss(&[1e6],&[-1e6]).unwrap(),2e6-2.0_f64.ln(),1e-8);
        close(loss(&[1.],&[2.]).unwrap(),1.0_f64.cosh().ln(),1e-12);
    }
    #[test]
    fn invalid() {
        assert!(loss(&[],&[]).is_err());
        assert!(loss(&[1.,2.],&[1.]).is_err());
        assert!(loss(&[1.],&[f64::NAN]).is_err());
        assert!(loss(&[1.],&[f64::INFINITY]).is_err());
        assert!(loss(&[1.],&[1_000_001.]).is_err());
        assert!(loss(&vec![0.;10_001],&vec![0.;10_001]).is_err());
    }
}
