//! Mean absolute error for finite real-valued regression data.

pub fn mean_absolute_error(truth: &[f64], predictions: &[f64]) -> Result<f64, &'static str> {
    if truth.is_empty() || truth.len() != predictions.len() {
        return Err("nonempty aligned vectors required");
    }
    let n = truth.len() as f64;
    let mut average = 0.0_f64;
    for (&y, &p) in truth.iter().zip(predictions) {
        if !y.is_finite() || !p.is_finite() { return Err("non-finite observation"); }
        let residual = y - p;
        if !residual.is_finite() { return Err("residual overflow"); }
        average += residual.abs() / n;
        if !average.is_finite() { return Err("average overflow"); }
    }
    Ok(average)
}

#[cfg(test)]
mod tests {
    use super::mean_absolute_error as mae;

    #[test]
    fn valid_examples() {
        assert!((mae(&[2.0,-1.0,5.0], &[1.0,1.0,3.0]).unwrap() - 5.0/3.0).abs() < 1e-12);
        assert_eq!(mae(&[2.0,-1.0], &[2.0,-1.0]), Ok(0.0));
        assert_eq!(mae(&[2.0], &[0.0]), Ok(2.0));
        assert_eq!(mae(&[1e308,1e308], &[0.0,0.0]), Ok(1e308));
    }

    #[test]
    fn invalid_inputs() {
        assert!(mae(&[], &[]).is_err());
        assert!(mae(&[1.0], &[]).is_err());
        assert!(mae(&[f64::NAN], &[0.0]).is_err());
        assert!(mae(&[f64::INFINITY], &[0.0]).is_err());
        assert!(mae(&[1e308], &[-1e308]).is_err());
    }
}
