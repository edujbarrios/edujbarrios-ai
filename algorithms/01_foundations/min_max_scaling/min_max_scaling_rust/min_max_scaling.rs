//! Min-max scaling to [0,1] for one finite, nonconstant feature.

pub fn min_max_scaling(values: &[f64]) -> Result<Vec<f64>, &'static str> {
    if values.is_empty() {
        return Err("input must be nonempty");
    }
    let (mut low, mut high) = (values[0], values[0]);
    for &value in values {
        if !value.is_finite() {
            return Err("input must be finite");
        }
        low = low.min(value);
        high = high.max(value);
    }
    let span = high - low;
    if !span.is_finite() || span <= 0.0 {
        return Err("input must be nonconstant with representable range");
    }
    let mut output = Vec::with_capacity(values.len());
    for &value in values {
        let normalized = if value == low {
            0.0
        } else if value == high {
            1.0
        } else {
            (value - low) / span
        };
        if !normalized.is_finite() {
            return Err("normalization overflow");
        }
        output.push(normalized.clamp(0.0, 1.0));
    }
    Ok(output)
}

#[cfg(test)]
mod tests {
    use super::min_max_scaling as scale;

    #[test]
    fn typical_and_unsorted() {
        assert_eq!(scale(&[-5.0,0.0,5.0]),Ok(vec![0.0,0.5,1.0]));
        assert_eq!(scale(&[10.0,20.0,30.0]),Ok(vec![0.0,0.5,1.0]));
        assert_eq!(scale(&[2.0,1.0]),Ok(vec![1.0,0.0]));
    }
    #[test]
    fn invalid() {
        assert!(scale(&[]).is_err());
        assert!(scale(&[1.0]).is_err());
        assert!(scale(&[7.0,7.0,7.0]).is_err());
        assert!(scale(&[1.0,f64::NAN]).is_err());
        assert!(scale(&[-1e308,1e308]).is_err());
    }
}
