//! Mean squared error with finite residual and output validation.

/// Calculate unweighted MSE between equally long, nonempty f64 arrays.
pub fn mean_squared_error(actual: &[f64], predicted: &[f64]) -> Result<f64, &'static str> {
    if actual.is_empty() || actual.len() != predicted.len() {
        return Err("equal nonempty input lengths required");
    }
    let mut average = 0.0_f64;
    for (i, (&y, &hat)) in actual.iter().zip(predicted).enumerate() {
        if !y.is_finite() || !hat.is_finite() {
            return Err("finite input values required");
        }
        let difference = y - hat;
        let squared = difference * difference;
        if !squared.is_finite() {
            return Err("squared residual overflow");
        }
        average += (squared - average) / (i + 1) as f64;
    }
    if !average.is_finite() {
        return Err("nonfinite mean squared error");
    }
    Ok(average.max(0.0))
}

#[cfg(test)]
mod tests {
    use super::mean_squared_error as mse;
    #[test]
    fn fixtures() {
        assert_eq!(mse(&[3.0, 0.0], &[1.0, 2.0]).unwrap(), 4.0);
        assert_eq!(mse(&[1.0, 2.0], &[1.0, 2.0]).unwrap(), 0.0);
        assert_eq!(mse(&[1.0, 2.0], &[2.0, 3.0]).unwrap(), 1.0);
        assert_eq!(mse(&[-3.0, 3.0], &[0.0, 0.0]).unwrap(), 9.0);
        assert_eq!(mse(&[5.0], &[-5.0]).unwrap(), 100.0);
        assert!((mse(&[1e154, 1e154], &[0.0, 0.0]).unwrap() / 1e308 - 1.0).abs() < 1e-12);
    }
    #[test]
    fn invalid() {
        assert!(mse(&[], &[]).is_err());
        assert!(mse(&[1.0], &[1.0, 2.0]).is_err());
        assert!(mse(&[f64::NAN], &[0.0]).is_err());
        assert!(mse(&[f64::INFINITY], &[0.0]).is_err());
        assert!(mse(&[1e308], &[0.0]).is_err());
    }
}
