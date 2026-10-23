//! Full-batch MSE gradient descent for univariate linear regression.

/// Train (weight, bias) from zero initialization for a fixed epoch count.
pub fn gradient_descent(
    x: &[f64], y: &[f64], learning_rate: f64, epochs: usize,
) -> Result<(f64, f64), &'static str> {
    if x.is_empty() || x.len() != y.len() ||
        epochs == 0 || !learning_rate.is_finite() || learning_rate <= 0.0 {
        return Err("invalid data dimensions or training settings");
    }
    if !x.iter().chain(y.iter()).all(|v| v.is_finite()) {
        return Err("observations must be finite");
    }
    let (mut weight, mut bias) = (0.0_f64, 0.0_f64);
    for _ in 0..epochs {
        let (mut gw, mut gb) = (0.0_f64, 0.0_f64);
        for (&a, &b) in x.iter().zip(y) {
            let prediction = weight * a + bias;
            let error = prediction - b;
            let component = error * a;
            if !prediction.is_finite() || !error.is_finite() || !component.is_finite() {
                return Err("non-finite training intermediate");
            }
            gw += component;
            gb += error;
            if !gw.is_finite() || !gb.is_finite() {
                return Err("gradient overflow");
            }
        }
        let factor = (2.0 * learning_rate) / x.len() as f64;
        weight -= factor * gw;
        bias -= factor * gb;
        if !factor.is_finite() || !weight.is_finite() || !bias.is_finite() {
            return Err("training diverged");
        }
    }
    Ok((weight, bias))
}

#[cfg(test)]
mod tests {
    use super::gradient_descent as train;

    #[test]
    fn fitted_line_and_horizontal_line() {
        let (w, b) = train(&[0.,1.,2.,3.], &[1.,3.,5.,7.], 0.05, 2000).unwrap();
        assert!((w-2.).abs()<1e-3 && (b-1.).abs()<1e-3);
        let (w, b) = train(&[0.,1.,2.,3.], &[3.,3.,3.,3.], 0.05, 2000).unwrap();
        assert!(w.abs()<1e-3 && (b-3.).abs()<1e-3);
    }

    #[test]
    fn invalid() {
        assert!(train(&[], &[], 0.05, 10).is_err());
        assert!(train(&[1.], &[], 0.05, 10).is_err());
        assert!(train(&[1.], &[2.], 0., 10).is_err());
        assert!(train(&[1.], &[2.], -0.1, 10).is_err());
        assert!(train(&[1.], &[2.], 0.05, 0).is_err());
        assert!(train(&[f64::NAN], &[2.], 0.05, 10).is_err());
    }
}
