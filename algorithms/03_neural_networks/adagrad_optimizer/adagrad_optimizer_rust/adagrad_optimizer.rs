//! One AdaGrad update using explicitly accumulated squared gradients.

/// Return updated parameters and accumulators, rejecting nonfinite intermediate
/// results. Epsilon appears outside sqrt(accumulator).
pub fn adagrad_optimizer(
    parameters: &[f64], gradients: &[f64], accumulator: &[f64],
    learning_rate: f64, epsilon: f64,
) -> Result<(Vec<f64>, Vec<f64>), &'static str> {
    if parameters.len() != gradients.len() || parameters.len() != accumulator.len() ||
        !learning_rate.is_finite() || learning_rate <= 0.0 ||
        !epsilon.is_finite() || epsilon <= 0.0 ||
        parameters.iter().any(|v| !v.is_finite()) ||
        gradients.iter().any(|v| !v.is_finite()) ||
        accumulator.iter().any(|v| !v.is_finite() || *v < 0.0) {
        return Err("invalid aligned finite inputs or hyperparameters");
    }
    let mut next_w = Vec::with_capacity(parameters.len());
    let mut next_h = Vec::with_capacity(parameters.len());
    for ((&w, &g), &h) in parameters.iter().zip(gradients).zip(accumulator) {
        let new_h = h + g * g;
        let denominator = new_h.sqrt() + epsilon;
        let step = learning_rate * g / denominator;
        let new_w = w - step;
        if !new_h.is_finite() || !denominator.is_finite() ||
            !step.is_finite() || !new_w.is_finite() {
            return Err("nonfinite AdaGrad intermediate");
        }
        next_w.push(new_w);
        next_h.push(new_h);
    }
    Ok((next_w, next_h))
}

#[cfg(test)]
mod tests {
    use super::adagrad_optimizer as step;

    #[test]
    fn updates_state() {
        let (w, h) = step(&[1.0, -1.0], &[2.0, -4.0], &[0.0, 0.0], .1, 1.0).unwrap();
        assert_eq!(h, vec![4.0, 16.0]);
        assert!((w[0] - (1.0 - .2 / 3.0)).abs() < 1e-12);
        assert!((w[1] - (-1.0 + .4 / 5.0)).abs() < 1e-12);
        let (w2, h2) = step(&w, &[2.0, 0.0], &h, .1, 1.0).unwrap();
        assert_eq!(h2, vec![8.0, 16.0]);
        assert_eq!(w2[1], w[1]);
        assert_eq!(step(&[], &[], &[], .1, 1.0), Ok((vec![], vec![])));
    }

    #[test]
    fn invalid_inputs() {
        assert!(step(&[1.0], &[], &[0.0], .1, 1.0).is_err());
        assert!(step(&[1.0], &[1.0], &[-1.0], .1, 1.0).is_err());
        assert!(step(&[f64::NAN], &[1.0], &[0.0], .1, 1.0).is_err());
        assert!(step(&[1.0], &[f64::INFINITY], &[0.0], .1, 1.0).is_err());
        assert!(step(&[1.0], &[1.0], &[0.0], 0.0, 1.0).is_err());
        assert!(step(&[1.0], &[1.0], &[0.0], .1, 0.0).is_err());
        assert!(step(&[1.0], &[1e308], &[0.0], .1, 1.0).is_err());
    }
}
