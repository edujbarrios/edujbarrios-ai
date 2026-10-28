//! Stable mean binary cross-entropy directly from raw classification logits.

/// Return unweighted mean BCE over finite logits and targets in [0, 1].
pub fn binary_cross_entropy_with_logits(
    logits: &[f64], targets: &[f64],
) -> Result<f64, &'static str> {
    let n = logits.len();
    if !(1..=10_000).contains(&n) || targets.len() != n {
        return Err("equal lengths in 1..10000 required");
    }
    let mut total = 0.0;
    for (&z, &y) in logits.iter().zip(targets.iter()) {
        if !z.is_finite() || z.abs() > 1_000_000.0 ||
           !y.is_finite() || !(0.0..=1.0).contains(&y) {
            return Err("finite bounded logits and targets in [0,1] required");
        }
        let term = z.max(0.0) - z*y + (-z.abs()).exp().ln_1p();
        total += term.max(0.0);
    }
    let mean = total / n as f64;
    if !mean.is_finite() { return Err("nonfinite result"); }
    Ok(mean)
}

#[cfg(test)]
mod tests {
    use super::binary_cross_entropy_with_logits as bce;
    fn close(a: f64, b: f64, tol: f64) {
        assert!((a-b).abs() <= tol, "{a} != {b}");
    }
    #[test]
    fn reference_values() {
        let expected = (2.0_f64.ln()+2.0*(-2.0_f64).exp().ln_1p()+2.0)/3.0;
        close(bce(&[0.,2.,-2.], &[0.,1.,1.]).unwrap(),expected,1e-12);
        close(bce(&[0.], &[0.5]).unwrap(),2.0_f64.ln(),1e-12);
        close(bce(&[1e6], &[1.]).unwrap(),0.,1e-12);
        close(bce(&[-1e6], &[1.]).unwrap(),1e6,1e-9);
        close(bce(&[1e6], &[0.5]).unwrap(),5e5,1e-9);
    }
    #[test]
    fn invalid() {
        assert!(bce(&[],&[]).is_err());
        assert!(bce(&[1.,2.],&[1.]).is_err());
        assert!(bce(&[0.],&[-0.1]).is_err());
        assert!(bce(&[0.],&[1.1]).is_err());
        assert!(bce(&[f64::NAN],&[1.]).is_err());
        assert!(bce(&[f64::INFINITY],&[1.]).is_err());
        assert!(bce(&[1_000_001.],&[1.]).is_err());
        assert!(bce(&[0.],&[f64::NAN]).is_err());
        assert!(bce(&vec![0.;10001],&vec![1.;10001]).is_err());
    }
}
