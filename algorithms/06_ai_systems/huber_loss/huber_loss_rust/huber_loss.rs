//! Mean Huber regression loss with quadratic and linear residual regimes.

pub fn huber_loss(actual: &[f64], predicted: &[f64],
                  delta: f64) -> Result<f64, &'static str> {
    if actual.is_empty() || actual.len() != predicted.len() ||
        !delta.is_finite() || delta <= 0.0 {
        return Err("equal nonempty lengths and positive finite delta required");
    }
    let mut mean = 0.0_f64;
    for (i, (&a, &b)) in actual.iter().zip(predicted).enumerate() {
        if !a.is_finite() || !b.is_finite() {
            return Err("finite real observations required");
        }
        let residual = (a - b).abs();
        if !residual.is_finite() { return Err("residual overflow"); }
        let value = if residual <= delta {
            0.5 * residual * residual
        } else {
            delta * (residual - 0.5 * delta)
        };
        if !value.is_finite() { return Err("loss overflow"); }
        mean += (value - mean) / (i + 1) as f64;
    }
    if !mean.is_finite() { return Err("nonfinite mean"); }
    Ok(mean.max(0.0))
}

#[cfg(test)]
mod tests {
    use super::huber_loss as huber;
    #[test]
    fn fixtures() {
        assert!((huber(&[1.5,4.0], &[1.0,2.0], 1.0).unwrap() - 0.8125).abs() < 1e-12);
        assert!((huber(&[1.5,4.0], &[1.0,2.0], 2.0).unwrap() - 1.0625).abs() < 1e-12);
        assert!((huber(&[1.5,4.0], &[1.0,2.0], 0.5).unwrap() - 0.5).abs() < 1e-12);
        assert_eq!(huber(&[1.0,2.0], &[1.0,2.0], 1.0).unwrap(), 0.0);
        assert_eq!(huber(&[-2.0], &[2.0], 1.0).unwrap(), 3.5);
        assert!((huber(&[1e154], &[0.0], 1.0).unwrap()/1e154 - 1.0).abs() < 1e-12);
    }
    #[test]
    fn invalid() {
        assert!(huber(&[], &[], 1.0).is_err());
        assert!(huber(&[1.0], &[1.0,2.0], 1.0).is_err());
        assert!(huber(&[f64::NAN], &[0.0], 1.0).is_err());
        assert!(huber(&[1.0], &[0.0], 0.0).is_err());
        assert!(huber(&[1.0], &[0.0], f64::INFINITY).is_err());
        assert!(huber(&[1e308], &[-1e308], 1.0).is_err());
        assert!(huber(&[1e308], &[0.0], 2.0).is_err());
    }
}
