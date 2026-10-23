//! Full-batch binary logistic regression for one numeric input feature.

fn sigmoid(z: f64) -> f64 {
    if z >= 0.0 {
        let e = (-z).exp();
        1.0 / (1.0 + e)
    } else {
        let e = z.exp();
        e / (1.0 + e)
    }
}

/// Return (weight, bias) after a fixed number of batch BCE-gradient updates.
pub fn logistic_regression(
    x: &[f64], y: &[f64], learning_rate: f64, epochs: usize,
) -> Result<(f64, f64), &'static str> {
    if x.is_empty() || x.len() != y.len() || epochs == 0 ||
       !learning_rate.is_finite() || learning_rate <= 0.0 {
        return Err("invalid sizes or training parameters");
    }
    if !x.iter().all(|v| v.is_finite()) ||
       !y.iter().all(|v| v.is_finite() && (*v == 0.0 || *v == 1.0)) {
        return Err("inputs must be finite, labels must be binary");
    }
    let (mut weight, mut bias) = (0.0_f64, 0.0_f64);
    for _ in 0..epochs {
        let (mut grad_weight, mut grad_bias) = (0.0_f64, 0.0_f64);
        for (&a, &label) in x.iter().zip(y) {
            let z = weight * a + bias;
            if !z.is_finite() {
                return Err("logit overflow");
            }
            let error = sigmoid(z) - label;
            let component = error * a;
            if !component.is_finite() {
                return Err("gradient overflow");
            }
            grad_weight += component;
            grad_bias += error;
            if !grad_weight.is_finite() || !grad_bias.is_finite() {
                return Err("gradient overflow");
            }
        }
        let factor = learning_rate / x.len() as f64;
        let delta_weight = factor * grad_weight;
        let delta_bias = factor * grad_bias;
        if !factor.is_finite() || !delta_weight.is_finite() || !delta_bias.is_finite() {
            return Err("update overflow");
        }
        weight -= delta_weight;
        bias -= delta_bias;
        if !weight.is_finite() || !bias.is_finite() {
            return Err("training diverged");
        }
    }
    Ok((weight, bias))
}

#[cfg(test)]
mod tests {
    use super::logistic_regression;

    #[test]
    fn binary_separation() {
        let (weight, bias) = logistic_regression(
            &[0.0, 1.0, 2.0, 3.0], &[0.0, 0.0, 1.0, 1.0], 0.2, 2000
        ).unwrap();
        assert!(weight > 0.0);
        assert!(bias < -2.0);
        assert!(weight * 3.0 + bias > 2.0);
    }

    #[test]
    fn one_epoch_and_invalid() {
        assert_eq!(logistic_regression(&[0.0], &[1.0], 0.2, 1), Ok((0.0, 0.1)));
        assert!(logistic_regression(&[], &[], 0.2, 1).is_err());
        assert!(logistic_regression(&[1.0], &[], 0.2, 1).is_err());
        assert!(logistic_regression(&[0.0,1.0], &[0.0,2.0], 0.2, 1).is_err());
        assert!(logistic_regression(&[f64::NAN], &[0.0], 0.2, 1).is_err());
        assert!(logistic_regression(&[0.0], &[1.0], 0.0, 1).is_err());
        assert!(logistic_regression(&[0.0], &[1.0], 0.2, 0).is_err());
        assert!(logistic_regression(&[1e308,0.0], &[0.0,1.0], 1e308, 20).is_err());
    }
}
