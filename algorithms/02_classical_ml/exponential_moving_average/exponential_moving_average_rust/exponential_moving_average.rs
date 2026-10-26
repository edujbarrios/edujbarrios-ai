//! Exponential moving average of a finite numeric sequence.

pub fn exponential_moving_average(values: &[f64], alpha: f64) -> Result<Vec<f64>, &'static str> {
    if values.is_empty() || !alpha.is_finite() || !(0.0 < alpha && alpha <= 1.0) {
        return Err("nonempty input and finite alpha in (0,1] required");
    }
    let mut result = Vec::with_capacity(values.len());
    for &x in values {
        if !x.is_finite() { return Err("non-finite observation"); }
        let next = match result.last() {
            None => x,
            Some(&previous) => (1.0 - alpha) * previous + alpha * x,
        };
        if !next.is_finite() { return Err("non-finite EMA"); }
        result.push(next);
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::exponential_moving_average as ema;

    #[test]
    fn expected_values() {
        assert_eq!(ema(&[10.0,20.0,10.0], 0.5), Ok(vec![10.0,15.0,12.5]));
        assert_eq!(ema(&[10.0,20.0,10.0], 1.0), Ok(vec![10.0,20.0,10.0]));
        assert_eq!(ema(&[7.0,7.0,7.0], 0.1), Ok(vec![7.0,7.0,7.0]));
        assert_eq!(ema(&[1e308,-1e308], 0.5), Ok(vec![1e308,0.0]));
    }

    #[test]
    fn invalid() {
        assert!(ema(&[], 0.5).is_err());
        assert!(ema(&[1.0,f64::NAN], 0.5).is_err());
        assert!(ema(&[1.0,f64::INFINITY], 0.5).is_err());
        assert!(ema(&[1.0], 0.0).is_err());
        assert!(ema(&[1.0], 1.1).is_err());
        assert!(ema(&[1.0], f64::NAN).is_err());
    }
}
