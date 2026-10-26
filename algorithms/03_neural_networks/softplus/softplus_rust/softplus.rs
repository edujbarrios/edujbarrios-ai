//! Stable Softplus activation and its sigmoid derivative.

/// Compute values and derivatives for finite, nonempty f64 vectors.
pub fn softplus(inputs: &[f64]) -> Result<(Vec<f64>, Vec<f64>), &'static str> {
    if inputs.is_empty() || !inputs.iter().all(|x| x.is_finite()) {
        return Err("nonempty finite real vector required");
    }
    let mut values = Vec::with_capacity(inputs.len());
    let mut derivatives = Vec::with_capacity(inputs.len());
    for &x in inputs {
        values.push(x.max(0.0) + (-x.abs()).exp().ln_1p());
        let d = if x >= 0.0 {
            1.0 / (1.0 + (-x).exp())
        } else {
            let z = x.exp();
            z / (1.0 + z)
        };
        derivatives.push(d);
    }
    Ok((values, derivatives))
}

#[cfg(test)]
mod tests {
    use super::softplus;
    #[test]
    fn reference_and_extreme_values() {
        let (v, d) = softplus(&[0.0, 1.0, -1.0, 1000.0, -1000.0]).unwrap();
        assert!((v[0] - 2.0_f64.ln()).abs() < 1e-12);
        assert_eq!(d[0], 0.5);
        assert!((v[1] - (1.0_f64.exp() + 1.0).ln()).abs() < 1e-12);
        assert!((v[2] - ((-1.0_f64).exp() + 1.0).ln()).abs() < 1e-12);
        assert!((d[1] - 1.0 / (1.0 + (-1.0_f64).exp())).abs() < 1e-12);
        assert!((d[2] - 1.0 / (1.0 + 1.0_f64.exp())).abs() < 1e-12);
        assert_eq!(&v[3..], &[1000.0, 0.0]);
        assert_eq!(&d[3..], &[1.0, 0.0]);
    }
    #[test]
    fn invalid() {
        assert!(softplus(&[]).is_err());
        assert!(softplus(&[f64::NAN]).is_err());
        assert!(softplus(&[f64::INFINITY]).is_err());
    }
}
