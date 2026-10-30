//! Stateless linear warmup and one cosine learning rate decay period.
/// Return the rate at step or an input validation error.
pub fn warmup_cosine_learning_rate(peak: f64, minimum: f64,
                                  warmup: usize, total: usize,
                                  step: usize) -> Result<f64, &'static str> {
    if !peak.is_finite() || !minimum.is_finite() ||
       minimum < 0.0 || minimum > peak || peak > 1_000_000.0 ||
       warmup == 0 || warmup >= total || total > 100_000 || step > total {
        return Err("invalid rates or step bounds");
    }
    if step <= warmup {
        return Ok(peak * (step as f64 / warmup as f64));
    }
    if step == total { return Ok(minimum); }
    let progress = (step - warmup) as f64 / (total - warmup) as f64;
    Ok(minimum + (peak - minimum) * 0.5 * (1.0 + (std::f64::consts::PI * progress).cos()))
}
#[cfg(test)]
mod tests {
    use super::warmup_cosine_learning_rate as rate;
    #[test] fn endpoints() {
        let expected = [0.0, 0.5, 1.0, 0.8535533905932737, 0.5, 0.1464466094067262, 0.0];
        for (step, &target) in expected.iter().enumerate() {
            assert!((rate(1.0,0.0,2,6,step).unwrap()-target).abs()<1e-12);
        }
        assert_eq!(rate(2.0,2.0,1,2,1), Ok(2.0));
        assert_eq!(rate(2.0,2.0,1,2,2), Ok(2.0));
    }
    #[test] fn invalid() {
        assert!(rate(1.0,0.0,0,6,1).is_err());
        assert!(rate(1.0,0.0,6,6,1).is_err());
        assert!(rate(1.0,0.0,2,6,7).is_err());
        assert!(rate(0.0,1.0,2,6,1).is_err());
        assert!(rate(f64::NAN,0.0,2,6,1).is_err());
        assert!(rate(1.0,0.0,2,100001,1).is_err());
    }
}
