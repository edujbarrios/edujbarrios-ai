//! Uniform scalar quantization to 8-bit unsigned bins.

pub fn scalar_quantization(
    values: &[f64], minimum: f64, maximum: f64
) -> Result<Vec<u8>, &'static str> {
    let span = maximum - minimum;
    if !minimum.is_finite() || !maximum.is_finite() ||
       !span.is_finite() || span <= 0.0 {
        return Err("invalid calibration range");
    }
    if !values.iter().all(|x| x.is_finite()) {
        return Err("input must contain finite values");
    }
    Ok(values.iter().map(|&value| {
        if value <= minimum { 0 }
        else if value >= maximum { 255 }
        else {
            let scaled = 255.0 * ((value - minimum) / span);
            (scaled + 0.5).floor().min(255.0) as u8
        }
    }).collect())
}

#[cfg(test)]
mod tests {
    use super::scalar_quantization as q;
    #[test]
    fn endpoint_and_midpoint() {
        assert_eq!(q(&[0.0,0.5,1.0,-3.0,8.0],0.0,1.0),
                   Ok(vec![0,128,255,0,255]));
        assert_eq!(q(&[],0.0,1.0),Ok(vec![]));
    }
    #[test]
    fn invalid() {
        assert!(q(&[f64::NAN],0.0,1.0).is_err());
        assert!(q(&[f64::INFINITY],0.0,1.0).is_err());
        assert!(q(&[0.0],1.0,1.0).is_err());
        assert!(q(&[0.0],-1e308,1e308).is_err());
        assert!(q(&[0.0],0.0,f64::INFINITY).is_err());
    }
}
