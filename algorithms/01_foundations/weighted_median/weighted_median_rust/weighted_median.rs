//! Lower weighted median for finite observations with nonnegative weights.

/// Smallest value at which cumulative weight reaches at least half the total.
pub fn weighted_median(values: &[f64], weights: &[f64])
    -> Result<f64, &'static str> {
    if !(1..=512).contains(&values.len()) || values.len() != weights.len() {
        return Err("expected equally long arrays of size 1..512");
    }
    let mut pairs = Vec::with_capacity(values.len());
    for (&value, &weight) in values.iter().zip(weights) {
        if !value.is_finite() || !weight.is_finite() || weight < 0.0 {
            return Err("expected finite values and nonnegative finite weights");
        }
        pairs.push((value, weight));
    }
    pairs.sort_by(|a, b| a.0.total_cmp(&b.0));
    let mut total = 0.0f64;
    for (_, weight) in &pairs {
        total += *weight;
        if !total.is_finite() {
            return Err("weight sum overflow");
        }
    }
    if total <= 0.0 {
        return Err("positive weight sum required");
    }
    let mut cumulative = 0.0;
    for (value, weight) in pairs {
        cumulative += weight;
        if cumulative >= total - cumulative {
            return Ok(value);
        }
    }
    Err("invalid weight sum")
}

#[cfg(test)]
mod tests {
    use super::weighted_median as wmedian;
    #[test]
    fn examples() {
        assert_eq!(wmedian(&[1.,2.,10.], &[1.,1.,8.]), Ok(10.));
        assert_eq!(wmedian(&[1.,2.,3.,4.], &[1.,1.,1.,1.]), Ok(2.));
        assert_eq!(wmedian(&[10.,1.,3.], &[0.,2.,1.]), Ok(1.));
        assert_eq!(wmedian(&[1.,100.], &[0.,2.]), Ok(100.));
        assert_eq!(wmedian(&[-5.,2.], &[1.,1.]), Ok(-5.));
        assert_eq!(wmedian(&[42.], &[0.01]), Ok(42.));
        assert_eq!(wmedian(&[3.;512], &[1.;512]), Ok(3.));
        assert_eq!(wmedian(&[-1e308,1e308], &[1.,1.]), Ok(-1e308));
        assert_eq!(wmedian(&[1.,2.], &[0.0, f64::from_bits(1)]), Ok(2.));
    }
    #[test]
    fn invalid() {
        assert!(wmedian(&[], &[]).is_err());
        assert!(wmedian(&[1.], &[]).is_err());
        assert!(wmedian(&[1.], &[0.]).is_err());
        assert!(wmedian(&[1.,2.], &[0.,0.]).is_err());
        assert!(wmedian(&[1.,2.], &[-1.,2.]).is_err());
        assert!(wmedian(&[1.], &[f64::INFINITY]).is_err());
        assert!(wmedian(&[f64::NAN], &[1.]).is_err());
        assert!(wmedian(&[1.,2.], &[1e308,1e308]).is_err());
        assert!(wmedian(&[1.;513], &[1.;513]).is_err());
    }
}
